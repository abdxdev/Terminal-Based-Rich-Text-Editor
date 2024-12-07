#include "Editor.h"

Editor::Editor() {
	// Cursor::hide();
	local_current_pos = { 0, 0 };
	left_gap = 6;
	is_file_saved = false;
	file_path = experimental::filesystem::current_path().string();
	file_name = "Untitled.txt";
	absolute_screen_size = Cursor::get_screen_size();
	top_bound = 3;
	left_bound = 9;
	right_bound = absolute_screen_size.second;
	bottom_bound = absolute_screen_size.first - 1;
	statusBar = StatusBar({ "Rich Text Editor", "abd" }, absolute_screen_size, file_path, file_name, local_current_pos, is_file_saved);
}

pair<int, int> Editor::getAbsPos(const pair<int, int>& pos) const {
	return { pos.first + top_bound, pos.second + left_bound + left_gap };
}

int Editor::getAbsRow(int row) const {
	return row + top_bound;
}

int Editor::getAbsCol(int col) const {
	return col + left_bound + left_gap;
}

Action Editor::handleKeyInput(int key) {
	Action action;
	if (key == 224) {
		switch (_getch()) {
		case 75:
			gapBufferEditor.moveCursorLeft();
			action = MOVE_CURSOR;
			break;
		case 77:
			gapBufferEditor.moveCursorRight();
			action = MOVE_CURSOR;
			break;
		case 72:
			gapBufferEditor.moveCursorUp();
			action = MOVE_CURSOR;
			break;
		case 80:
			gapBufferEditor.moveCursorDown();
			action = MOVE_CURSOR;
			break;
		case 83:
			gapBufferEditor.deleteCharRight();
			action = DELETE;
			break;
		default:
			break;
		}
	}
	else {
		switch (key) {
		case 27:
			exit(0);
			break;
		case 8:
			gapBufferEditor.deleteCharLeft();
			action = DELETE;
			break;
		case 9:
			gapBufferEditor.insert('\t');
			action = INSERT;
			break;
		case '\r':
			gapBufferEditor.insert('\n');
			action = INSERT;
			break;
		default:
			if (key >= 32 && key <= 126)
				gapBufferEditor.insert(static_cast<char>(key));
			action = INSERT;
			break;
		}
	}
	return action;
}

void Editor::renderAllLinesFromCursor() {
	int height = bottom_bound - top_bound;
	int lines_needed = height - local_current_pos.first;

	int from = gapBufferEditor.getCursorRow();
	int to = min(from + lines_needed, gapBufferEditor.getLinesCount() - 1);

	vector<string> lines = gapBufferEditor.getLines(from, to);
	for (int i = from; i <= to; i++) {
		Cursor::move_to({ getAbsRow(i), left_bound });
		renderLineNumber(i);
		cout << lines[i - from];
		Cursor::clear_line_from_cursor();
	}
}

void Editor::renderLineNumber(int row) {
	cout << TextFormatter::hex_to_ansi("#777777") << setw(left_gap) << to_string(row + 1) + "  " << TextFormatter::RESET;
}

void Editor::render() {
	auto abs_pos = getAbsPos(local_current_pos);
	Cursor::move_to({ abs_pos.first, left_bound });
	renderLineNumber(gapBufferEditor.getCursorRow());

	Cursor::move_to({ absolute_screen_size.first, 1 });
	statusBar.updateCursorPos(local_current_pos);
	statusBar.display();
}

void Editor::run() {
	while (true) {
		try {
			render();
			Cursor::move_to(getAbsPos(local_current_pos));
			Cursor::show();
			int key = _getch();
			if (key == 27) { // ESC key
				system("cls");
				break;
			}
			Action action = handleKeyInput(key);
			// Cursor::hide();

			int rowChange = gapBufferEditor.getCursorRow() - local_current_pos.first;
			int colChange = gapBufferEditor.getCursorColumn() - local_current_pos.second;

			if (!rowChange && !colChange && action != INSERT && action != DELETE) {
				continue;
			}
			local_current_pos = gapBufferEditor.getCursorPosition();
			if (action == MOVE_CURSOR)
				continue;

			//DebugUtils::displayDebugMessage("No change in cursor position",
			//	{ bottom_bound, left_bound });

			Cursor::clear_line_from_cursor();
			if (rowChange > 0) {
				renderAllLinesFromCursor();
			}
			else if (rowChange < 0) {
				renderAllLinesFromCursor();
			}
			else if (colChange > 0) {
				gapBufferEditor.displayCurrentLine(gapBufferEditor.getCursorColumn() - colChange, -1);
			}
			else if (colChange < 0) {
				Cursor::move_left(abs(colChange));
				gapBufferEditor.displayCurrentLine(gapBufferEditor.getCursorColumn(), -1);
				cout << setw(abs(colChange)) << " ";
			}
		}
		catch (const char* e) {
			DebugUtils::displayDebugMessage(e, { bottom_bound, left_bound });
		}
	}
}
