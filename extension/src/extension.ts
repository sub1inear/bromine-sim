
import * as vscode from 'vscode';
import * as fs from 'fs';
import axios from 'axios';
import * as path from 'path';
import * as fflate from 'fflate';
import { pipeline } from 'stream/promises';
import { spawn } from 'child_process';
import { once } from 'events';
import { writeBuildFiles } from './build';
import { getTemplateSourceUrl, readTemplates } from './templates';

type ProgressReporter = (percent: number, message: string) => void;

function createProgressReporter(progress: vscode.Progress<{ message?: string; increment?: number }>) {
	let reported = 0;
	return (start: number, end: number): ProgressReporter => (percent, message) => {
		const overall = Math.floor(start + (end - start) * Math.min(percent, 100) / 100);
		if (overall > reported) {
			progress.report({ increment: overall - reported, message });
			reported = overall;
		}
	};
}

async function downloadFile(url: string, outputPath: string, signal: AbortSignal, onProgress: ProgressReporter): Promise<void> {
	const response = await axios.get(url, { responseType: 'stream', signal });
	const total = Number(response.headers['content-length']) || 0;
	let loaded = 0;

	response.data.on('data', (chunk: Buffer) => {
		loaded += chunk.length;
		if (total) {
			onProgress(loaded / total * 100, `Downloading ${(loaded / 1024 / 1024).toFixed(1)} / ${(total / 1024 / 1024).toFixed(1)} MB`);
		}
	});

	await pipeline(response.data, fs.createWriteStream(outputPath), { signal });
}

async function extractZip(filePath: string, outputPath: string, onProgress: ProgressReporter): Promise<void> {
	const root = path.resolve(outputPath);
	const total = (await fs.promises.stat(filePath)).size;
	let read = 0;
	let error: Error | undefined;
	let pending: Promise<void>[] = [];

	const unzipper = new fflate.Unzip(file => {
		const fullPath = path.resolve(root, file.name);
		if (!fullPath.startsWith(root + path.sep)) {
			error = new Error(`Archive entry escapes the output directory: ${file.name}`);
			return;
		}
		if (file.name.endsWith('/')) {
			pending.push(fs.promises.mkdir(fullPath, { recursive: true }).then(() => { }));
			return;
		}
		const chunks: Uint8Array[] = [];
		file.ondata = (err, data, final) => {
			if (err) {
				error = err;
				return;
			}
			chunks.push(data);
			if (final) {
				pending.push((async () => {
					await fs.promises.mkdir(path.dirname(fullPath), { recursive: true });
					await fs.promises.writeFile(fullPath, Buffer.concat(chunks), { mode: 0o755 });
				})());
			}
		};
		file.start();
	});
	unzipper.register(fflate.UnzipInflate);

	const flush = async () => {
		if (error) {
			throw error;
		}
		await Promise.all(pending);
		pending = [];
	};

	for await (const chunk of fs.createReadStream(filePath)) {
		unzipper.push(chunk);
		await flush();
		read += chunk.length;
		onProgress(read / total * 100, `Extracting ${path.basename(filePath)}`);
	}
	unzipper.push(new Uint8Array(0), true);
	await flush();
}

async function extractTar(filePath: string, outputPath: string, onProgress: ProgressReporter): Promise<void> {
	const total = (await fs.promises.stat(filePath)).size;
	let read = 0;
	const tar = spawn('tar', [filePath.endsWith('.xz') ? '-xJf' : '-xzf', '-', '-C', outputPath], { stdio: ['pipe', 'ignore', 'inherit'] });
	const input = fs.createReadStream(filePath);
	input.on('data', chunk => {
		read += chunk.length;
		onProgress(read / total * 100, `Extracting ${path.basename(filePath)}`);
	});
	const [[code]] = await Promise.all([once(tar, 'close'), pipeline(input, tar.stdin)]);
	if (code !== 0) {
		throw new Error(`Extracting ${path.basename(filePath)} failed with exit code ${code}`);
	}
}

async function extractArchive(filePath: string, outputPath: string, onProgress: ProgressReporter): Promise<void> {
	if (filePath.endsWith('.zip')) {
		await extractZip(filePath, outputPath, onProgress);
	} else if (/\.tar\.(gz|xz)$/.test(filePath)) {
		await extractTar(filePath, outputPath, onProgress);
	} else {
		throw new Error(`Unsupported archive format: ${filePath}`);
	}
}

async function flattenSingleRootDir(dir: string): Promise<void> {
	const entries = await fs.promises.readdir(dir, { withFileTypes: true });
	if (entries.length !== 1 || !entries[0].isDirectory()) {
		return;
	}
	const rootDir = path.join(dir, entries[0].name);
	for (const name of await fs.promises.readdir(rootDir)) {
		await fs.promises.rename(path.join(rootDir, name), path.join(dir, name));
	}
	await fs.promises.rmdir(rootDir);
}

async function installTool(name: string, url: string, installDir: string): Promise<boolean> {
	const marker = path.join(installDir, '.installed');
	if (fs.existsSync(marker) && fs.readFileSync(marker, 'utf8') === url) {
		return true;
	}

	return vscode.window.withProgress(
		{
			location: vscode.ProgressLocation.Notification,
			title: `Installing ${name}`,
			cancellable: true
		},
		async (progress, token) => {
			const controller = new AbortController();
			token.onCancellationRequested(() => controller.abort());
			const phase = createProgressReporter(progress);

			await fs.promises.rm(installDir, { recursive: true, force: true });
			await fs.promises.mkdir(installDir, { recursive: true });
			console.log(`${name} installation path: ${installDir}`);

			const archivePath = path.join(installDir, path.basename(new URL(url).pathname));
			try {
				await downloadFile(url, archivePath, controller.signal, phase(0, 50));
			} catch (error) {
				await fs.promises.rm(archivePath, { force: true });
				if (controller.signal.aborted) {
					vscode.window.showInformationMessage(`${name} installation was canceled.`);
					return false;
				}
				throw error;
			}

			await extractArchive(archivePath, installDir, phase(50, 100));
			await fs.promises.rm(archivePath, { force: true });
			await flattenSingleRootDir(installDir);
			await fs.promises.writeFile(marker, url);
			return true;
		}
	);
}

function getToolPaths(context: vscode.ExtensionContext) {
	const globalStoragePath = context.globalStorageUri.fsPath;
	const exe = process.platform === 'win32' ? '.exe' : '';
	const zigDir = path.join(globalStoragePath, 'zig');
	const ninjaDir = path.join(globalStoragePath, 'ninja');
	return {
		zigDir,
		ninjaDir,
		coreDir: path.join(globalStoragePath, 'bromine-sim-core'),
		templatesDir: path.join(globalStoragePath, 'templates'),
		zig: path.join(zigDir, `zig${exe}`),
		ninja: path.join(ninjaDir, `ninja${exe}`),
	};
}

async function installDependencies(context: vscode.ExtensionContext): Promise<boolean> {
	const { zigDir, ninjaDir, coreDir } = getToolPaths(context);
	const installed = await Promise.all([
		installTool('Zig', getZigUrl('0.17.0'), zigDir),
		installTool('Ninja', getNinjaUrl('1.13.2'), ninjaDir),
		installTool('Bromine Sim Core', await getBromineSimCoreUrl(coreDir), coreDir),
	]);
	return installed.every(Boolean);
}

async function installTemplates(context: vscode.ExtensionContext, project: string): Promise<{ name: string; dir: string }[] | undefined> {
	const dirs = [];
	for (const template of await readTemplates(project)) {
		const id = `${template.name}@${template.version}`;
		const dir = path.join(getToolPaths(context).templatesDir, id);
		if (!fs.existsSync(path.join(dir, '.installed')) && !await installTool(id, await getTemplateSourceUrl(template), dir)) {
			return undefined;
		}
		dirs.push({ name: template.name, dir });
	}
	return dirs;
}

async function runBuild(project: string, ninja: string, buildDir: string): Promise<number | undefined> {
	const folder = vscode.workspace.getWorkspaceFolder(vscode.Uri.file(project)) ?? vscode.TaskScope.Workspace;
	const task = new vscode.Task(
		{ type: 'bromine-sim' }, folder, 'Build Sim', 'bromine-sim',
		new vscode.ProcessExecution(ninja, ['-C', buildDir]),
		[]
	);
	task.presentationOptions = { clear: true };
	let execution: vscode.TaskExecution | undefined;
	return new Promise((resolve, reject) => {
		const listener = vscode.tasks.onDidEndTaskProcess(event => {
			if (event.execution === execution) {
				listener.dispose();
				resolve(event.exitCode);
			}
		});
		vscode.tasks.executeTask(task).then(result => execution = result, error => {
			listener.dispose();
			reject(error);
		});
	});
}

let simTerminal: vscode.Terminal | undefined;

let installing: Promise<boolean> | undefined;

function ensureDependencies(context: vscode.ExtensionContext): Promise<boolean> {
	installing ??= installDependencies(context).finally(() => installing = undefined);
	return installing;
}

function getZigUrl(version: string): string {
	if (process.platform === 'win32' && process.arch === 'x64') {
		return `https://ziglang.org/download/${version}/zig-x86_64-windows-${version}.zip`;
	} else if (process.platform === 'linux' && process.arch === 'x64') {
		return `https://ziglang.org/download/${version}/zig-x86_64-linux-${version}.tar.xz`;
	} else if (process.platform === 'darwin' && process.arch === 'arm64') {
		return `https://ziglang.org/download/${version}/zig-aarch64-macos-${version}.tar.xz`;
	} else {
		throw new Error(`Unsupported platform: ${process.platform}-${process.arch}`);
	}
}

function getNinjaUrl(version: string): string {
	if (process.platform === 'win32' && process.arch === 'x64') {
		return `https://github.com/ninja-build/ninja/releases/download/v${version}/ninja-win.zip`;
	} else if (process.platform === 'linux' && process.arch === 'x64') {
		return `https://github.com/ninja-build/ninja/releases/download/v${version}/ninja-linux.zip`;
	} else if (process.platform === 'darwin' && process.arch === 'arm64') {
		return `https://github.com/ninja-build/ninja/releases/download/v${version}/ninja-mac.zip`;
	} else {
		throw new Error(`Unsupported platform: ${process.platform}-${process.arch}`);
	}
}
function getBromineSimCoreAssetName(): string {
	if (process.platform === 'win32' && process.arch === 'x64') {
		return 'bromine-sim-windows-x64.zip';
	} else if (process.platform === 'linux' && process.arch === 'x64') {
		return 'bromine-sim-linux-x64.tar.gz';
	} else if (process.platform === 'darwin' && process.arch === 'arm64') {
		return 'bromine-sim-macos-arm64.tar.gz';
	} else {
		throw new Error(`Unsupported platform: ${process.platform}-${process.arch}`);
	}
}

async function getBromineSimCoreUrl(installDir: string): Promise<string> {
	const latest = `https://github.com/sub1inear/bromine-sim/releases/latest/download/${getBromineSimCoreAssetName()}`;
	try {
		const response = await axios.head(latest, { maxRedirects: 0, validateStatus: status => status === 302, timeout: 5000 });
		return response.headers.location;
	} catch (error) {
		const marker = path.join(installDir, '.installed');
		if (fs.existsSync(marker)) {
			return fs.readFileSync(marker, 'utf8');
		}
		throw error;
	}
}

async function findProjects(): Promise<string[]> {
	const files = await vscode.workspace.findFiles('**/project.pros', '**/{node_modules,.bromine-sim}/**');
	return files.map(file => path.dirname(file.fsPath)).sort();
}

async function pickProject(): Promise<string | undefined> {
	const projects = await findProjects();
	if (projects.length <= 1) {
		if (!projects.length) {
			vscode.window.showErrorMessage('No PROS project (project.pros) found in this workspace.');
		}
		return projects[0];
	}
	const picked = await vscode.window.showQuickPick(
		projects.map(project => ({ label: vscode.workspace.asRelativePath(project), project })),
		{ placeHolder: 'Select a PROS project to simulate' }
	);
	return picked?.project;
}

export function activate(context: vscode.ExtensionContext) {
	const updateProjects = async () => {
		const hasProject = (await findProjects()).length > 0;
		await vscode.commands.executeCommand('setContext', 'bromine-sim.hasProject', hasProject);
	};

	const watcher = vscode.workspace.createFileSystemWatcher('**/project.pros', false, true, false);
	watcher.onDidCreate(updateProjects);
	watcher.onDidDelete(updateProjects);

	const run = vscode.commands.registerCommand('bromine-sim.run', async () => {
		if (!context.storageUri) {
			vscode.window.showErrorMessage('Open a folder to run the simulator.');
			return;
		}
		const project = await pickProject();
		if (!project) {
			return;
		}
		try {
			if (!await ensureDependencies(context)) {
				return;
			}
		} catch (error) {
			vscode.window.showErrorMessage(`Failed to install dependencies: ${(error as Error).message}`);
			return;
		}

		try {
			const templateDirs = await installTemplates(context, project);
			if (!templateDirs) {
				return;
			}
			const { zig, ninja, coreDir } = getToolPaths(context);
			const buildDir = path.join(context.storageUri.fsPath, 'build');
			const exe = await writeBuildFiles(project, buildDir, zig, coreDir, templateDirs);
			if (await runBuild(project, ninja, buildDir) !== 0) {
				return;
			}
			simTerminal?.dispose();
			simTerminal = vscode.window.createTerminal({ name: 'Bromine Sim', shellPath: exe, cwd: project });
			simTerminal.show();
		} catch (error) {
			vscode.window.showErrorMessage(`Failed to run the simulator: ${(error as Error).message}`);
		}
	});

	context.subscriptions.push(
		watcher,
		run,
		vscode.workspace.onDidChangeWorkspaceFolders(updateProjects),
		vscode.window.onDidCloseTerminal(terminal => {
			if (terminal === simTerminal) {
				simTerminal = undefined;
			}
		}),
	);
	updateProjects();
}

export function deactivate() { }
