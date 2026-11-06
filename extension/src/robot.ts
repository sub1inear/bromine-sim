import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';

const defaultRobot = {
	version: 1,
	width: 18.0,
	height: 18.0,
	mass: 18.0,
	devices: [],
};

export async function ensureRobotConfig(project: string): Promise<boolean> {
	const robotPath = path.join(project, 'robot.json');
	if (fs.existsSync(robotPath)) {
		return true;
	}
	const create = 'Create robot.json';
	const choice = await vscode.window.showWarningMessage(
		`No robot.json found in ${vscode.workspace.asRelativePath(project)}. Would you like to create one?`,
		{ modal: true, detail: 'The simulator uses a robot.json file to describe your robot. An empty configuration will be created.' },
		create
	);
	if (choice !== create) {
		return false;
	}
	await fs.promises.writeFile(robotPath, JSON.stringify(defaultRobot, null, 4) + '\n');
	await vscode.window.showTextDocument(vscode.Uri.file(robotPath), { preview: false });
	return true;
}
