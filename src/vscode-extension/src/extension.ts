import * as vscode from "vscode";
import { exec } from "child_process";

type Position = [number, number];
type Range = [Position, Position];

enum Tag {
	SYMBOL_TYPE_TAG = "PM_SYMBOL_TYPE_LOC",
	DIAGNOSTIC_TAG = "PM_DIAGNOSTIC_LOC"
}

enum DiagnosticLevel {
	WARNING = "WARNING",
	ERROR = "ERROR"
}

interface SymbolType {
	fileName: string;
	position: Position;
	pointerType: string;
}

interface Diagnostic {
	fileName: string;
	range: Range;
	message: string;
	level: DiagnosticLevel;
}

const parseClangRepsonse = (response: string) => {
	const lines = response.split("\n");
	const symbolTypes: SymbolType[] = [];
	const diagnostics: Diagnostic[] = [];
	for (let line of lines) {
		line = line.trim();

		if (!line.startsWith(Tag.DIAGNOSTIC_TAG) && !line.startsWith(Tag.SYMBOL_TYPE_TAG)) {
			continue;
		}

		const parts = line.split(";");
		const tag = parts[0] as Tag;
		if (tag === Tag.SYMBOL_TYPE_TAG) {
			const fileName = parts[1];
			const [linePos, colPos] = parts[2].split(",").map(Number);
			const pointerType = parts[3];

			symbolTypes.push({
				fileName,
				position: [linePos, colPos],
				pointerType
			});
		}

		if (tag === Tag.DIAGNOSTIC_TAG) {
			const fileName = parts[1];
			const [linePos1, colPos1] = parts[2].split(",").map(Number);
			const [linePos2, colPos2] = parts[3].split(",").map(Number);
			const range: Range = [[linePos1, colPos1], [linePos2, colPos2]];
			const level = parts[4] as DiagnosticLevel;
			const message = parts[5];

			diagnostics.push({
				fileName,
				range,
				message,
				level
			});
		}
	}

	return {
		symbolTypes,
		diagnostics
	};
}

const getBuildCommand = (fileName: string) => {
	// Fix hardcoded path
	const pmWrapperPath = "/home/teppichseite/development/thesis/pm-wrapper/PmWrapperCompiler";
	return [
		'PM_WRAPPER_OUTPUT_PATH=/tmp/output',
		`${pmWrapperPath}`,
		fileName
	].join(" ");
}

const runCommand = (command: string) => new Promise<string>((resolve) => exec(command,
	(error, stdout, _) => {
		if (error) {
			resolve(stdout);
		} else {
			resolve(stdout);
		}
	}));

const getClangResponse = async (fileName: string) => {
	const command = getBuildCommand(fileName);
	const result = await runCommand(command).catch(err => err);
	return parseClangRepsonse(result);
}

const setDiagnostics = (
	document: vscode.TextDocument,
	diagnosticCollection: vscode.DiagnosticCollection,
	diagnostics: Diagnostic[]) => {

	diagnosticCollection.clear();

	diagnosticCollection.set(
		vscode.Uri.file(document.fileName),
		diagnostics.map(diagnostic => {
			const [[l1, c1], [l2, c2]] = diagnostic.range;

			return new vscode.Diagnostic(
				new vscode.Range(
					new vscode.Position(l1 - 1, c1 - 1),
					new vscode.Position(l2 - 1, c2)),
				diagnostic.message,
				diagnostic.level === DiagnosticLevel.ERROR
					? vscode.DiagnosticSeverity.Error
					: vscode.DiagnosticSeverity.Warning)
		}));
};

const getInlineHints = (document: vscode.TextDocument, symbolTypes: SymbolType[]) => symbolTypes
	.filter(v => v.fileName === document.fileName)
	.map(({ pointerType, position }) => {
		const hint = new vscode.InlayHint(
			new vscode.Position(position[0] - 1, position[1] - 1),
			`${pointerType}:`, vscode.InlayHintKind.Parameter);

		hint.paddingLeft = false;
		hint.paddingRight = true;

		return hint;
	});

export function activate(context: vscode.ExtensionContext) {

	const diagnosticCollection = vscode.languages.createDiagnosticCollection("pm-wrapper-sense-diagnostics");

	const hintProviderDisposable = vscode.languages.registerInlayHintsProvider('c', {
		async provideInlayHints(document) {
			const { symbolTypes, diagnostics } = await getClangResponse(document.fileName);

			setDiagnostics(document, diagnosticCollection, diagnostics);

			return getInlineHints(document, symbolTypes);
		}
	});

	context.subscriptions.push(diagnosticCollection);
	context.subscriptions.push(hintProviderDisposable);
}

export function deactivate() { }
