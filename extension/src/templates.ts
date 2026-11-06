import * as fs from 'fs';
import * as os from 'os';
import * as path from 'path';
import axios from 'axios';

export interface Template {
	name: string;
	version: string;
	origin: string;
}

interface DepotTemplate {
	name: string;
	version: string;
	metadata: { location: string };
}

function getProsConfigDir(): string {
	if (process.platform === 'win32') {
		return path.join(process.env.APPDATA ?? path.join(os.homedir(), 'AppData', 'Roaming'), 'PROS');
	} else if (process.platform === 'darwin') {
		return path.join(os.homedir(), 'Library', 'Application Support', 'PROS');
	}
	return path.join(process.env.XDG_CONFIG_HOME ?? path.join(os.homedir(), '.config'), 'pros');
}

export async function readTemplates(project: string): Promise<Template[]> {
	const json = JSON.parse(await fs.promises.readFile(path.join(project, 'project.pros'), 'utf8'));
	const templates: Record<string, { version: string; metadata: { origin: string } }> = json['py/state']?.templates ?? {};
	return Object.entries(templates)
		.filter(([name]) => name !== 'kernel' && name !== 'liblvgl')
		.map(([name, template]) => ({ name, version: template.version, origin: template.metadata.origin }));
}

async function getDepotLocation(origin: string): Promise<string> {
	const conductor = path.join(getProsConfigDir(), 'conductor.pros');
	const json = JSON.parse(await fs.promises.readFile(conductor, 'utf8'));
	const location = json['py/state']?.depots?.[origin]?.location;
	if (!location) {
		throw new Error(`Depot ${origin} not found in ${conductor}`);
	}
	return location;
}

export async function getTemplateSourceUrl(template: Template): Promise<string> {
	const depotLocation = await getDepotLocation(template.origin);
	const depot: DepotTemplate[] = (await axios.get(depotLocation)).data;
	const match = depot.find(entry => entry.name === template.name && entry.version === template.version);
	if (!match) {
		throw new Error(`Template ${template.name}@${template.version} not found in depot ${template.origin} at ${depotLocation}`);
	}

	const location = new URL(match.metadata.location);
	if (location.hostname !== 'github.com') {
		throw new Error(`Expected a github.com source for template ${template.name}: ${location}`);
	}
	const [author, repo, , , tag] = location.pathname.split('/').filter(Boolean);
	return `https://github.com/${author}/${repo}/archive/refs/tags/${tag}.zip`;
}
