#include "Editor.h"
#include "AutoSuggestion.h"
#include "ColorSelector.h"

Editor::Editor() : absolute_screen_size(Cursor::get_screen_size()), topBar({}, "#ffffff", "#000000", false, absolute_screen_size, 2) {
    local_current_pos = {0, 0};
    vertical_scroll = 0;
    horizontal_scroll = 0;
    left_line_number_gap = 20;
    is_file_saved = false;
    file_path = experimental::filesystem::current_path().string();
    file_name = "Untitled.txt";
    top_bound = 3 + 1;
    left_bound = 3;
    right_bound = absolute_screen_size.second;
    bottom_bound = absolute_screen_size.first - 1;
    statusBar = StatusBar({"Rich Text Editor", "windows"}, absolute_screen_size, file_path, file_name, local_current_pos, is_file_saved);
}

pair<int, int> Editor::getAbsPos(const pair<int, int>& pos) const {
    return {pos.first + top_bound, pos.second + left_bound + left_line_number_gap};
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
        switch (_getch()) {
        case 59: // F1
        {
            int height = 15;
            int width = 30;
            string selectedColor = ColorSelector::selectColor(
                {(absolute_screen_size.first - height) / 2,
                 (absolute_screen_size.second - width) / 2},
                height,
                width);

            topBar.changeForegroundColor(TextFormatter::ansi_to_hex(selectedColor));
            gapBufferEditor.insert(selectedColor);
        }
            return REFRESH;
        case 60: // F2
        {
            int height = 15;
            int width = 30;
            string selectedColor = ColorSelector::selectColor(
                {(absolute_screen_size.first - height) / 2,
                 (absolute_screen_size.second - width) / 2},
                height,
                width);

            topBar.changeBackgroundColor(TextFormatter::ansi_to_hex(selectedColor));
            gapBufferEditor.insert(TextFormatter::convert_to_bg(selectedColor));
        }
            return REFRESH;
        case 61: // F3
            topBar.toggleAutoSuggestion();
            return AUTOSUGGESTION;
        case 62: // F4
            return FULL_REFRESH;
        case 63: // F5
            topBar.toggleCursorStyle();
            Cursor::change_cursor_style(topBar.cursorStyle);
        }
    } else if (key == 224) {
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
        case 83:          // delete key
            return OTHER; // TODO: Implement delete key
            gapBufferEditor.deleteCharRight();
            return DELETE;
        }
    } else {
        switch (key) {
        case 8: // backspace key
            gapBufferEditor.deleteCharLeft();
            return DELETE;
        // case 9: // tab key
        // 	gapBufferEditor.insert("    ");
        // 	return INSERT;
        case '\r': // Enter key
            gapBufferEditor.insert('\n');
            return INSERT;
        case 2: // Ctrl + B
            topBar.toggleFormat(eFormats::BOLD);
            gapBufferEditor.insert(topBar.getCurrentFormatting(eFormats::BOLD));
            return FORMAT;
        case 9: // Ctrl + I
            topBar.toggleFormat(eFormats::ITALIC);
            gapBufferEditor.insert(topBar.getCurrentFormatting(eFormats::ITALIC));
            return FORMAT;
        case 21: // Ctrl + U
            topBar.toggleFormat(eFormats::UNDERLINE);
            gapBufferEditor.insert(topBar.getCurrentFormatting(eFormats::UNDERLINE));
            return FORMAT;
        case 20: // Ctrl + T
            topBar.toggleFormat(eFormats::STRIKETHROUGH);
            gapBufferEditor.insert(topBar.getCurrentFormatting(eFormats::STRIKETHROUGH));
            return FORMAT;
        case 26: // Ctrl + Z
            gapBufferEditor.undo();
            renderAllLines(local_current_pos.first);
            local_current_pos = { gapBufferEditor.getCursorRow(), gapBufferEditor.getCursorColumn() };
            return INSERT;
        case 25: // Ctrl + Y
            gapBufferEditor.redo();
            renderAllLines(local_current_pos.first);
            local_current_pos = { gapBufferEditor.getCursorRow(), gapBufferEditor.getCursorColumn() };
            return INSERT;
        case 19: // Ctrl + S
            gapBufferEditor.save(statusBar.file_name);
            statusBar.updateIsFileSaved(true);
            return SOFT_REFRESH;
        case 27: // ESC key
            return BREAK;
        default:
            if ((('A' <= key && key <= 'Z') || ('a' <= key && key <= 'z')) && topBar.isAutoSuggestionOn) {
                string word = string(1, key);
                bool isComplete = AutoSuggestion::getInput(4, word, getAbsPos(local_current_pos), 30);
                gapBufferEditor.insert(word);
                if (isComplete)
                    gapBufferEditor.insert(' ');
                return INSERT;
            } else if (key >= 32 && key <= 126) {
                gapBufferEditor.insert(static_cast<char>(key));
                return INSERT;
            }
            return OTHER;
        }
    }
}

void Editor::refreshScreen(bool screenChanged) {
    system("cls");
    if (screenChanged) {
        this->absolute_screen_size = Cursor::get_screen_size();
        this->right_bound = absolute_screen_size.second;
        this->bottom_bound = absolute_screen_size.first - 1;
        this->statusBar.updateScreenSize(absolute_screen_size);
		this->topBar.updateScreenSize(absolute_screen_size);
    }
    topBar.display();
    renderAllLines(0);
    Cursor::move_to(getAbsPos(local_current_pos));
}

void Editor::renderAllLines(int from) {
    int height = bottom_bound - top_bound;
    int to = min(height, gapBufferEditor.getLinesCount() - 1);

    Cursor::clear_screen_from_cursor();

    vector<string> lines = gapBufferEditor.getLines(from, to);
    for (int i = from; i <= to; i++) {
        Cursor::move_to({getAbsRow(i), left_bound});
        renderLineNumber(i);
        cout << lines[i - from];
        Cursor::clear_line_from_cursor();
    }
}

void Editor::renderLineNumber(int row) const {
    cout << TextFormatter::hex_to_ansi("#777777") << setw(left_line_number_gap) << right << to_string(row + 1) + "  " << TextFormatter::RESET;
}

void Editor::render() {
    Cursor::move_to({getAbsRow(local_current_pos.first), left_bound});
    renderLineNumber(gapBufferEditor.getCursorRow());

    Cursor::move_to({absolute_screen_size.first, 1});
    statusBar.updateCursorPos(local_current_pos);
    statusBar.display();
}

void Editor::run() {
    topBar.display();
    Cursor::change_cursor_style(topBar.cursorStyle);
    while (true) {
        try {
            render();
            Cursor::move_to(getAbsPos(local_current_pos));
            Cursor::show();
            Action action = handleKeyInput();
            Cursor::hide();
            if (action == BREAK) {
                system("cls");
                break;
            } else if (action == OTHER) {
                continue;
            } else if (action == FULL_REFRESH) {
                refreshScreen(true);
                continue;
            } else if (action == REFRESH) {
                refreshScreen(false);
                continue;
            } else if (action == FORMAT || action == AUTOSUGGESTION) {
                Cursor::move_to({0, 0});
                topBar.display();
                Cursor::move_to(getAbsPos(local_current_pos));
                continue;
            }

            int rowChange = gapBufferEditor.getCursorRow() - local_current_pos.first;
            int colChange = gapBufferEditor.getCursorColumn() - local_current_pos.second;

            if (!rowChange && !colChange && action != INSERT && action != DELETE)
                continue;

            local_current_pos = gapBufferEditor.getCursorPosition();

            if (action == MOVE_CURSOR)
                continue;

            Cursor::clear_line_from_cursor();

            statusBar.updateIsFileSaved(false);
            
            if (rowChange > 0) {
                renderAllLines(local_current_pos.first);
            } else if (rowChange < 0) {
                renderAllLines(local_current_pos.first);
            }
            else if (colChange > 0) {
                Cursor::move_to({getAbsRow(local_current_pos.first), getAbsCol(0)});
                gapBufferEditor.displayCurrentLine();
                cout << TextFormatter::RESET;
            }
            else if (colChange < 0) {
                Cursor::move_to({getAbsRow(local_current_pos.first), getAbsCol(0)});
                gapBufferEditor.displayCurrentLine();
                cout << TextFormatter::RESET;
                cout << setw(abs(colChange)) << " ";
            } else if (action == INSERT) {
                renderAllLines(local_current_pos.first);
            } else if (action == DELETE) {
                renderAllLines(local_current_pos.first);
            }
            //  DebugUtils::displayDebugMessage(gapBufferEditor.getCurrentDebugLine(), { bottom_bound, left_bound });
        } catch (const char* e) {
            DebugUtils::displayDebugMessage(e, {bottom_bound, left_bound});
        }
    }
}
