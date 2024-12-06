#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "Cursor.h"
#include "GapBuffer.h"
#include "Test.h"
#include "TextFormatter.h"
#include "Utils.h"
#include <conio.h>
#include <experimental/filesystem>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sstream>
#include <io.h>

using namespace std;
using namespace Cursor;
using namespace TextFormatter;
using namespace Utils;
namespace filesystem = experimental::filesystem;

class Editor {
public:
	string help_keybind;
	string help_message;
	string file_name;
	string editor_title;
	bool is_file_saved;
	//pair<int, int> current_pos;
	pair<int, int> absolute_screen_size;
	vector<wstring> left_status_bar_entries;

	Editor() {
		absolute_screen_size = get_screen_size();
		//current_pos = { 1, 1 };
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
			//L"Ln " + to_wstring(1) + L", Col " + to_wstring(1)
			L"Ln 1, Col 1"
		};
		printStatusBar({ 1, 1 });
	}

	wstring get_file_path(const wstring& delimiter, int max_folders = 0) {
		wstring current_path = filesystem::current_path().wstring();
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
			}
			else {
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

		move_to({ absolute_screen_size.first, 1 });
		_setmode(_fileno(stdout), _O_U16TEXT);
		wcout << text_to_display;
		_setmode(_fileno(stdout), _O_TEXT);
		cout << RESET;
		clear_line_from_cursor();
		move_to(current_pos);
	}

	void debugMessage(pair<int, int> current_pos, pair<int, int> target_pos, string message) {
		move_to(target_pos);
		cout << message;
		clear_line_from_cursor();
		move_to(current_pos);
	}
};

int main() {
	// CheckKeys();
	cout << "Make terminal full screen and press enter";
	_getch();
	system("cls");

	GapBuffer editor;
	int key;
	bool running = true;

	Editor e;

	while (running) {
		show();
		key = _getch();
		hide();

		if (key == 224) {
			//do {
			//	key = _getch();
			//} while (key == 224);

			//switch (key) {
			switch (_getch()) {
			case 75:
				if (editor.getCursorPosition() > 1) {
					editor.moveCursorRelative(-1);
				}
				break;
			case 77:
				if (editor.getCursorPosition() <= editor.getLength()) {
					editor.moveCursorRelative(1);
				}
				break;
			default:
				break;
			}
		}
		else {
			switch (key) {
			case 27:
				running = false;
				break;
			case 8:
				if (editor.getCursorPosition() > 1) {
					editor.moveCursorRelative(-1);
					editor.deleteChar();
				}
				break;
			default:
				if (key >= 32 && key <= 126) {
					editor.insert(static_cast<char>(key));
				}
				break;
			}
		}

		move_to({ 1, 1 });
		clear_line_from_cursor();
		editor.display();
		e.debugMessage({ 1, editor.getCursorPosition() }, { 2, 1 }, "Cursor position: " + editor.getDebugText());
		e.printStatusBar({ 1, editor.getCursorPosition() });
	}
	return 0;
}
