
import * as vscode from 'vscode';
import * as fs from 'fs';
import axios from 'axios';
import * as path from 'path';
import * as fflate from 'fflate';
import { pipeline } from 'stream/promises';
import { spawn } from 'child_process';
import { once } from 'events';

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

async function installDependencies(context: vscode.ExtensionContext): Promise<boolean> {
	const globalStoragePath = context.globalStorageUri.fsPath;
	const installed = await Promise.all([
		installTool('Zig', getZigUrl('0.17.0'), path.join(globalStoragePath, 'zig')),
		installTool('Ninja', getNinjaUrl('1.13.2'), path.join(globalStoragePath, 'ninja')),
	]);
	return installed.every(Boolean);
}

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

export function activate(context: vscode.ExtensionContext) {
	const disposable = vscode.commands.registerCommand('bromine-sim.run', async () => {
		try {
			if (!await ensureDependencies(context)) {
				return;
			}
		} catch (error) {
			vscode.window.showErrorMessage(`Failed to install dependencies: ${(error as Error).message}`);
			return;
		}
	});

	context.subscriptions.push(disposable);
}

export function deactivate() { }
