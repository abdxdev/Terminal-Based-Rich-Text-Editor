#pragma once

#include "Cursor.h"
#include "TextFormatter.h"
#include "Trie.h"
#include "Utils.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

class AutoSuggestion {

public:
	static bool getInput(int maxSuggestions, string& token, pair<int, int> pos, int width) {
		string fg = "#26bdbb";
		string bg = "#0c212f";

		int key = token[0];
		token = "";
		bool stop = false;
		int selected = 0;
		int column_width = 0;
		vector<string> suggested;
		int cursor_pos = 0;
		bool isComplete = false;

		do {
			Cursor::move_to({ pos.first, pos.second + cursor_pos });
			if (key == 0)
				key = _getch();

			if (key == 224) {
				switch (_getch()) {
				case 72: // up arrow key
					selected--;
					break;
				case 80: // down arrow key
					selected++;
					break;
				}
				if (selected < 0)
					selected = suggested.size() - 1;
				else if (selected >= suggested.size())
					selected = 0;
				key = 0;
			}
			else {
				switch (key) {
				case 8: // backspace key
					if (token.length()) {
						cout << "\b \b";
						token = token.substr(0, token.length() - 1);
						cursor_pos--;
					}
					break;
				case 27: // ESC key
					isComplete = false;
					stop = true;
					break;
				case 9:    // tab key
				case '\r': // Enter key
					token = (suggested.empty()) ? token : suggested[selected];
				case ' ':
					isComplete = true;
					stop = true;
					break;
				default:
					if (' ' <= key && key <= '~') {
						token += static_cast<char>(key);
						cout << static_cast<char>(key);
						cursor_pos++;
					}
				}
				if (token.length() == 0)
					suggested.clear();
				else {
					if (!stop)
						suggested = Trie::words.getSuggestions(token, maxSuggestions);
					else
						suggested.clear();
					column_width = min(width, Utils::max_column_width(suggested) + 2);
				}
				key = 0;
			}

			for (int i = 0; i < suggested.size(); i++) {
				Cursor::move_to({ pos.first + i + 1, pos.second });
				cout << TextFormatter::get_format(fg, bg, true) << left << setw(column_width) << ' ' + suggested[i] << TextFormatter::RESET << string(width - column_width, ' ');
			}
			for (int i = suggested.size(); i < maxSuggestions; i++) {
				Cursor::move_to({ pos.first + i + 1, pos.second });
				cout << left << setw(width) << ' ';
			}

			if (!suggested.empty()) {
				selected = min(selected, int(suggested.size() - 1));
				Cursor::move_to({ pos.first + selected + 1, pos.second });
				cout << TextFormatter::get_format(bg, fg) << left << setw(column_width) << ' ' + suggested[selected] << TextFormatter::RESET;
			}

		} while (!stop);
		//Cursor::move_to(pos);
		//cout << setw(token.length()) << ' ';
		Cursor::move_to(pos);
		return isComplete;
	}
};