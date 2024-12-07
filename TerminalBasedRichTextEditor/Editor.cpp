#include "Editor.h"

Editor::Editor() {
	// Cursor::hide();
	local_current_pos = { 0, 0 };
	vertical_scroll = 0;
	horizontal_scroll = 0;
	left_line_number_gap = 6;
	is_file_saved = false;
	file_path = experimental::filesystem::current_path().string();
	file_name = "Untitled.txt";
	absolute_screen_size = Cursor::get_screen_size();
	top_bound = 3;
	left_bound = 1;
	right_bound = absolute_screen_size.second;
	bottom_bound = absolute_screen_size.first - 1;
	statusBar = StatusBar({ "Rich Text Editor", "abd" }, absolute_screen_size, file_path, file_name, local_current_pos, is_file_saved);
}

pair<int, int> Editor::getAbsPos(const pair<int, int>& pos) const {
	return { pos.first + top_bound, pos.second + left_bound + left_line_number_gap };
}

int Editor::getAbsRow(int row) const {
	return row + top_bound;
}

int Editor::getAbsCol(int col) const {
	return col + left_bound + left_line_number_gap;
}

Action Editor::handleKeyInput() {
	int key = _getch();
	if (key == 0) {
		return OTHER;
	}
	else if (key == 224) {
		switch (_getch()) {
		case 75: // left arrow key
			gapBufferEditor.moveCursorLeft();
			return MOVE_CURSOR;
		case 77: // right arrow key
			gapBufferEditor.moveCursorRight();
			return MOVE_CURSOR;
		case 72: // up arrow key
			gapBufferEditor.moveCursorUp();
			return MOVE_CURSOR;
		case 80: // down arrow key
			gapBufferEditor.moveCursorDown();
			return MOVE_CURSOR;
		case 83: // delete key
			return OTHER; // TODO: Implement delete key
			gapBufferEditor.deleteCharRight();
			return DELETE;
		}
	}
	else {
		switch (key) {
		case 8: // backspace key
			gapBufferEditor.deleteCharLeft();
			return DELETE;
		case 9: // tab key
			gapBufferEditor.insert("    ");
			return INSERT;
		case '\r': // Enter key
			gapBufferEditor.insert('\n');
			return INSERT;
		case 12:  // Ctrl + L
			refreshScreen();
			return OTHER;
		case 27: // ESC key
			return BREAK;
		default:
			if (key >= 32 && key <= 126)
				gapBufferEditor.insert(static_cast<char>(key));
			return INSERT;
		}
	}
}

void Editor::refreshScreen() {
	system("cls");
	this->absolute_screen_size = Cursor::get_screen_size();
	this->right_bound = absolute_screen_size.second;
	this->bottom_bound = absolute_screen_size.first - 1;
	this->statusBar.updateScreenSize(absolute_screen_size);
	renderAllLines(0);
	Cursor::move_to(getAbsPos(local_current_pos));
	render();
}

void Editor::renderAllLines(int from) {
	int height = bottom_bound - top_bound;
	int to = min(height, gapBufferEditor.getLinesCount() - 1);

	Cursor::clear_screen_from_cursor();

	vector<string> lines = gapBufferEditor.getLines(from, to);
	for (int i = from; i <= to; i++) {
		Cursor::move_to({ getAbsRow(i), left_bound });
		renderLineNumber(i);
		cout << lines[i - from];
		Cursor::clear_line_from_cursor();
	}
}

void Editor::renderLineNumber(int row) const {
	cout << TextFormatter::hex_to_ansi("#777777") << setw(left_line_number_gap) << to_string(row + 1) + "  " << TextFormatter::RESET;
}

void Editor::render() {
	Cursor::move_to({ getAbsRow(local_current_pos.first), left_bound });
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
			// Cursor::show();
			Action action = handleKeyInput();
			// Cursor::hide();
			if (action == BREAK) {
				system("cls");
				break;
			}

			int rowChange = gapBufferEditor.getCursorRow() - local_current_pos.first;
			int colChange = gapBufferEditor.getCursorColumn() - local_current_pos.second;

			if (!rowChange && !colChange && action != INSERT && action != DELETE)
				continue;

			local_current_pos = gapBufferEditor.getCursorPosition();

			if (action == MOVE_CURSOR)
				continue;

			Cursor::clear_line_from_cursor();
			if (rowChange > 0) {
				renderAllLines(local_current_pos.first);
			}
			else if (rowChange < 0) {
				renderAllLines(local_current_pos.first);
			}
			else if (colChange > 0) {
				gapBufferEditor.displayCurrentLine(gapBufferEditor.getCursorColumn() - colChange, -1);
			}
			else if (colChange < 0) {
				Cursor::move_left(abs(colChange));
				gapBufferEditor.displayCurrentLine(gapBufferEditor.getCursorColumn(), -1);
				cout << setw(abs(colChange)) << " ";
			}
			else if (action == INSERT) {
				renderAllLines(local_current_pos.first);
			}
			else if (action == DELETE) {
				renderAllLines(local_current_pos.first);
			}
		}
		catch (const char* e) {
			DebugUtils::displayDebugMessage(e, { bottom_bound, left_bound });
		}
	}
}
