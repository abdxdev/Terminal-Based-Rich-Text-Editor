#pragma once

#include <conio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace Cursor {
	void move_up(const int& n);
	void move_down(const int& n);
	void move_right(const int& n);
	void move_left(const int& n);
	void move_to(const pair<int, int>& pos);
	void move_to_start_of_line();
	void show();
	void hide();
	void clear_line_to_cursor();
	void clear_line_from_cursor();
	void clear_line();
	void remove_n_lines(const int& n);
	void clear_screen_to_cursor();
	void clear_screen_from_cursor();
	void clear_screen();
	void save_position();
	void restore_position();
	void save_screen();
	void restore_screen();
	void save_screen_alt();
	void restore_screen_alt();
	void change_cursor_style(int i);
	pair<int, int> get_position();
	pair<int, int> get_screen_size();
};