#include "Editor.h"

Editor::Editor() {
    Cursor::hide();
    local_current_pos = {0, 0};
    left_gap = 6;
    is_file_saved = false;
    file_path = experimental::filesystem::current_path().string();
    file_name = "Untitled.txt";
    absolute_screen_size = Cursor::get_screen_size();
    top_bound = 3;
    left_bound = 9;
    right_bound = absolute_screen_size.second;
    bottom_bound = absolute_screen_size.first - 1;
    statusBar = StatusBar({
                              "Rich Text Editor",
                              "abd",
                          },
                          absolute_screen_size, file_path, file_name, local_current_pos, is_file_saved);
}

pair<int, int> Editor::getAbsPos(const pair<int, int>& pos) const {
    return {pos.first + top_bound, pos.second + left_bound + left_gap};
}

int Editor::getAbsRow(int row) const {
    return row + top_bound;
}

int Editor::getAbsCol(int col) const {
    return col + left_bound + left_gap;
}

void Editor::handleKeyInput(int key) {
    if (key == 224) {
        switch (_getch()) {
        case 75:
            gapBufferEditor.moveCursorLeft();
            break;
        case 77:
            gapBufferEditor.moveCursorRight();
            break;
        case 72:
            gapBufferEditor.moveCursorUp();
            break;
        case 80:
            gapBufferEditor.moveCursorDown();
            break;
        case 83:
            gapBufferEditor.deleteCharRight();
            break;
        default:
            break;
        }
    } else {
        switch (key) {
        case 27:
            exit(0);
            break;
        case 8:
            gapBufferEditor.deleteCharLeft();
            break;
        case 9:
            gapBufferEditor.insert('\t');
            break;
        case '\r':
            gapBufferEditor.insert('\n');
            break;
        default:
            if (key >= 32 && key <= 126)
                gapBufferEditor.insert(static_cast<char>(key));
            break;
        }
    }
    local_current_pos = gapBufferEditor.getCursorPosition();
}

void Editor::renderLineNumber() {
    cout << setw(left_gap) << to_string(gapBufferEditor.getCursorRow() + 1) + ": ";
}

void Editor::render() {
    auto abs_pos = getAbsPos(local_current_pos);

    // DebugUtils::displayDebugMessage("Cursor position: " + to_string(local_current_pos.first) + ", " + to_string(local_current_pos.second), {bottom_bound, left_bound});

    Cursor::move_to({abs_pos.first, left_bound});
    renderLineNumber();

    Cursor::move_to({absolute_screen_size.first, 1});
    statusBar.updateCursorPos(local_current_pos);
    statusBar.display();
}

void Editor::run() {
    while (true) {
        render();
        Cursor::move_to(getAbsPos(local_current_pos));
        Cursor::show();
        int key = _getch();
        if (key == 27) { // ESC key
            system("cls");
            break;
        }
        Cursor::hide();
        handleKeyInput(key);

        Cursor::move_to({getAbsRow(gapBufferEditor.getCursorRow()), left_bound + left_gap});
        Cursor::clear_line_from_cursor();
        gapBufferEditor.displayCurrentLine();
    }
}
