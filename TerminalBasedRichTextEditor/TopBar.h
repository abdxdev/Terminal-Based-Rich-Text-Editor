#pragma once

#include "Cursor.h"
#include "TextFormatter.h"
#include "Utils.h"
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class TopBar {
private:
    vector<string> custom_entries;
    vector<string> entries;
    map<string, int> entry_positions;
    pair<int, int> screen_size;
    string file_path;
    string file_name;
    bool is_file_saved;
    pair<int, int> local_cursor_pos;
    int status_bar_length;

    string formatEntries();

public:
    TopBar() = default;
    TopBar(vector<string> static_entries, pair<int, int> screen_size, string file_path, string file_name, pair<int, int> local_cursor_pos, bool is_file_saved);
    string getFormattedFilePath(const string& delimiter, int max_folders);
    void buildEntries();
    void updateFileName(string file_name);
    void updateCursorPos(pair<int, int> local_cursor_pos);
    void updateIsFileSaved(bool is_file_saved);
    void updateFilePath(string file_path);
    void updateScreenSize(pair<int, int> screen_size);
    void display();
};
