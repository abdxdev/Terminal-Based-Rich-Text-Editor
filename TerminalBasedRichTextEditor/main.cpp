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
	bool isSavedFile;
	pair<int, int> current_pos;
	pair<int, int> absolute_screen_size;

	Editor() {
		absolute_screen_size = get_screen_size();
		isSavedFile = false;
		help_keybind = "F1";
		help_message = "Press " + help_keybind + " for help";
		file_name = "Untitled.txt";
		editor_title = "Rich Text Editor";
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
		return path_str;
	}

	void printStatusBar() {
		// customization
		// #0c212f
		// #26bdbb
		// wstring text_to_display = L"    " + stows(editor_title) + L"    source  repos  TerminalBasedRichTextEditor  Untitled.txt*  Ln 50, Col 129";
		// cout << hex_to_ansi("#000000") + hex_to_ansi("#3dabff", false) << setw(pos.second) <<  << RESET;
		// cout << get_format("#000000", "#3dabff") << setw(absolute_screen_size.second) << editor_title << RESET;
		//     Rich Text Editor    source  repos  TerminalBasedRichTextEditor  Untitled.txt*  Ln 50, Col 129
		vector<wstring> left_status_bar_entries{
			L"",
			stows(editor_title),
			get_file_path(L"  ", 3),
			stows(file_name),
		};

		// vector<wstring> right_status_bar_entries{
		// 	L" ",
		// 	L"Ln 50, Col 129",
		// 	L"",
		// };

		move_to({ absolute_screen_size.first, 1 });

		string default_color = "#000000";
		string accent_color1 = "#26bdbb";
		string accent_color2 = "#0c212f";

		_setmode(_fileno(stdout), _O_U16TEXT);
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
			if (i == 0) {
				text_to_display += stows(get_format(bg, default_color)) + L"" + stows(get_format(fg, bg));
			}
			else {
				text_to_display += stows(get_format(default_color, bg)) + L"" + stows(get_format(fg, bg));
			}
			text_to_display += L" "+left_status_bar_entries[i] + L" ";
			text_to_display += stows(get_format(bg, default_color));
			if (i < left_status_bar_entries.size() - 1) {
				text_to_display += L"";
			}
			else {
				text_to_display += L"";
			}
		}
		wcout << text_to_display << stows(RESET);
		_setmode(_fileno(stdout), _O_TEXT);

		move_to(current_pos);
	}

	void startTyping() {
	}
};

int main() {
	// CheckKeys();
	GapBuffer editor;
	int key;
	bool running = true;

	Editor e;
	e.printStatusBar();

	while (running) {
		key = _getch();

		if (key == 224) {
			do {
				key = _getch();
			} while (key == 224);

			switch (key) {
			case 75:
				editor.moveCursorRelative(-1);
				e.current_pos.second--;
				break;
			case 77:
				editor.moveCursorRelative(1);
				e.current_pos.second++;
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
				editor.deleteChar();
				e.current_pos.second--;
				break;
			default:
				if (key >= 32 && key <= 126) {
					editor.insert(static_cast<char>(key));
					e.current_pos.second++;
				}
				break;
			}
		}

		move_to({ e.current_pos.first, 1 });
		clear_line_from_cursor();
		editor.display();

		move_to(e.current_pos);
	}

	cout << "Exiting editor. Goodbye!\n";
	return 0;
}
