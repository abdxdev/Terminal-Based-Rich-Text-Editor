#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "Cursor.h"
#include "GapBufferLinkedList.h"
#include "Test.h"
#include "TextFormatter.h"
#include "Utils.h"
#include <conio.h>
#include <experimental/filesystem>
#include <fcntl.h>
#include <io.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace Cursor;
using namespace TextFormatter;
using namespace Utils;
namespace fs = experimental::filesystem;

class Editor {
public:
    int left_gap = 6; // for line numbers
    string help_keybind;
    string help_message;
    string file_name;
    string editor_title;
    bool is_file_saved;
    // pair<int, int> current_pos;
    pair<int, int> absolute_screen_size;
    pair<int, int> top_left_bound;
    pair<int, int> bottom_right_bound;
    vector<wstring> left_status_bar_entries;

    Editor() {
        top_left_bound = {3, 9};
        absolute_screen_size = get_screen_size();
        bottom_right_bound = {absolute_screen_size.first - 1, absolute_screen_size.second};
        // current_pos = { 1, 1 };
        is_file_saved = false;
        help_keybind = "F1";
        help_message = "Press " + help_keybind + " for help";
        file_name = "Untitled.txt";
        editor_title = "Rich Text Editor";
        left_status_bar_entries = {
            L"",
            stows(editor_title),
            get_file_path(L"  ", 3),
            stows(file_name),
            L"Ln 1, Col 1",
        };
    }

    wstring get_file_path(const wstring& delimiter, int max_folders = 0) {
        wstring current_path = fs::current_path().wstring();
        wstringstream wss(current_path);
        wstring segment;
        vector<wstring> path_segments;

        while (getline(wss, segment, L'\\')) {
            path_segments.push_back(segment);
        }
        if (max_folders > 0 && max_folders < path_segments.size()) {
            path_segments.erase(path_segments.begin(), path_segments.end() - max_folders);
        }
        wstring path_str = L" ";
        for (size_t i = 0; i < path_segments.size(); ++i) {
            path_str += delimiter + path_segments[i];
        }
        return L"\u001b]8;;" + current_path + L"\u001b\\" + path_str + L"\u001b]8;;\u001b\\";
    }

    void printStatusBar(pair<int, int> current_pos) {

        left_status_bar_entries[3] = stows(file_name) + (is_file_saved ? L"" : L"*");
        left_status_bar_entries[4] = L"Ln " + to_wstring(current_pos.first) + L", Col " + to_wstring(current_pos.second);

        string default_color = "#000000";
        string accent_color1 = "#26bdbb";
        string accent_color2 = "#0c212f";
        wstring text_to_display = L"";
        for (int i = 0; i < left_status_bar_entries.size(); ++i) {
            string fg, bg;
            if ((i & 1) == 0) {
                fg = accent_color1;
                bg = accent_color2;
            } else {
                fg = accent_color2;
                bg = accent_color1;
            }
            if (i == 0)
                text_to_display += stows(get_format(bg, default_color)) + L"" + stows(get_format(fg, bg));
            else
                text_to_display += stows(get_format(default_color, bg)) + L"" + stows(get_format(fg, bg));
            text_to_display += L" " + left_status_bar_entries[i] + L" ";
            text_to_display += stows(get_format(bg, default_color));
            if (i < left_status_bar_entries.size() - 1)
                text_to_display += L"";
            else
                text_to_display += L"";
        }

        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << text_to_display;
        _setmode(_fileno(stdout), _O_TEXT);
        cout << RESET;
        clear_line_from_cursor();
    }

    pair<int, int> get_abs_pos(pair<int, int> pos) {
        return {pos.first + top_left_bound.first, pos.second + top_left_bound.second + left_gap};
    }

    int get_abs_row(int row) {
        return row + top_left_bound.first;
    }

    int get_abs_col(int col) {
        return col + top_left_bound.second + left_gap;
    }

    int get_abs_right_bound() {
        return bottom_right_bound.second;
    }

    int get_abs_left_bound() {
        return top_left_bound.second + left_gap;
    }

    int get_abs_top_bound() {
        return top_left_bound.first;
    }

    int get_abs_bottom_bound() {
        return bottom_right_bound.first;
    }

    void debugMessage(pair<int, int> current_pos, pair<int, int> target_pos, string message) {
        move_to(target_pos);
        cout << message;
        clear_line_from_cursor();
        move_to(get_abs_pos(current_pos));
    }
};

int main() {
    // CheckKeys();
    cout << "Make terminal full screen and press enter";
    _getch();
    system("cls");

    GapBufferLinkedList gapBufferEditor;
    int key;
    bool running = true;

    Editor editorInstance;

    while (running) {
        pair<int, int> abs_pos = editorInstance.get_abs_pos(gapBufferEditor.getCursorPosition());
        move_to({editorInstance.absolute_screen_size.first, 1});
        editorInstance.printStatusBar(gapBufferEditor.getCursorPosition());
        move_to({abs_pos.first, editorInstance.get_abs_left_bound() - editorInstance.left_gap});
        cout << setw(editorInstance.left_gap) << to_string(gapBufferEditor.getCursorRow() + 1) + ": ";
        move_to(abs_pos);
        try {
            show();
            key = _getch();
            hide();
            if (key == 224) {
                switch (_getch()) {
                case 75: // Left arrow
                    gapBufferEditor.moveCursorLeft();
                    break;
                case 77: // Right arrow
                    gapBufferEditor.moveCursorRight();
                    break;
                case 72: // Up arrow
                    gapBufferEditor.moveCursorUp();
                    break;
                case 80: // Down arrow
                    gapBufferEditor.moveCursorDown();
                    break;
                case 83: // Delete key
                    gapBufferEditor.deleteCharRight();
                    break;
                default:
                    break;
                }
            } else {
                switch (key) {
                case 27:
                    running = false;
                    break;
                case 8: // Backspace
                    gapBufferEditor.deleteCharLeft();
                    break;
                case '\r': // Enter
                    gapBufferEditor.insert('\n');
                    break;
                case 9: // Tab
                    gapBufferEditor.insert('\t');
                    break;
                default:
                    if (key >= 32 && key <= 126)
                        gapBufferEditor.insert(static_cast<char>(key));
                    break;
                }
            };
            move_to({editorInstance.get_abs_row(gapBufferEditor.getCursorRow()), editorInstance.get_abs_left_bound()});
            clear_line_from_cursor();
            gapBufferEditor.displayCurrentLine();
        } catch (const char* ex) {
            vector<string> lines = {
                string(ex),
                gapBufferEditor.currentLine->gapBuffer.getDebugText(),
                to_string(editorInstance.bottom_right_bound.first),
                to_string(editorInstance.top_left_bound.second),
            };

            string message = "";

            for (int i = 0; i < lines.size(); ++i) {
                message += lines[i] + "  ";
            }
            
            editorInstance.debugMessage({gapBufferEditor.getCursorPosition()}, {editorInstance.bottom_right_bound.first, editorInstance.top_left_bound.second}, message);
        }
    }
    system("cls");
    return 0;
}
