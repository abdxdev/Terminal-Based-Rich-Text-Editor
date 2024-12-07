#include "StatusBar.h"

StatusBar::StatusBar() {}
StatusBar::StatusBar(vector<string> custom_entries, pair<int, int> screen_size, string file_path, string file_name, pair<int, int> local_cursor_pos, bool is_file_saved) {
    this->custom_entries = custom_entries;
    this->screen_size = screen_size;
    this->file_path = file_path;
    this->file_name = file_name;
    this->local_cursor_pos = local_cursor_pos;
    this->is_file_saved = is_file_saved;
    buildEntries();
}

void StatusBar::updateFileName(string file_name) {
    this->file_name = file_name;
    entries[custom_entries.size() + entry_positions["file_name"] - 1] = file_name + (is_file_saved ? "" : "*");
}

void StatusBar::updateCursorPos(pair<int, int> local_cursor_pos) {
    this->local_cursor_pos = local_cursor_pos;
    entries[custom_entries.size() + entry_positions["cursor_pos"] - 1] = "Ln " + to_string(local_cursor_pos.first + 1) + ", Col " + to_string(local_cursor_pos.second + 1);
}

void StatusBar::updateIsFileSaved(bool is_file_saved) {
    this->is_file_saved = is_file_saved;
    entries[custom_entries.size() + entry_positions["file_name"] - 1] = file_name + (is_file_saved ? "" : "*");
}

void StatusBar::updateFilePath(string file_path) {
    this->file_path = file_path;
    entries[custom_entries.size() + entry_positions["file_path"] - 1] = getFormattedFilePath("  ", 2);
}

void StatusBar::updateScreenSize(pair<int, int> screen_size) {
    this->screen_size = screen_size;
}

void StatusBar::display() {
    string formatted_entries = formatEntries();
    Utils::printWide(Utils::stows(formatted_entries));
    cout << TextFormatter::RESET;
    Cursor::clear_line_from_cursor();
}

void StatusBar::buildEntries() {
    for (int i = 0; i < custom_entries.size(); ++i) {
        entries.push_back(custom_entries[i]);
    }
    entry_positions = {
        {"custom_entries", 0},
        {"file_path", 1},
        {"file_name", 2},
        {"cursor_pos", 3},
    };
    entries.push_back(getFormattedFilePath("  ", 1));
    entries.push_back(file_name + (is_file_saved ? "" : "*"));
    entries.push_back("Ln " + to_string(local_cursor_pos.first) + ", Col " + to_string(local_cursor_pos.second));
}

string StatusBar::formatEntries() {
    string text_to_display = "";
    string default_color = "#000000";
    string accent_color1 = "#26bdbb";
    string accent_color2 = "#0c212f";
    for (int i = 0; i < entries.size(); ++i) {
        string fg, bg;
        if ((i & 1) == 0) {
            fg = accent_color1;
            bg = accent_color2;
        } else {
            fg = accent_color2;
            bg = accent_color1;
        }
        if (i == 0)
            text_to_display += TextFormatter::get_format(bg, default_color) + "" + TextFormatter::get_format(fg, bg);
        else
            text_to_display += TextFormatter::get_format(default_color, bg) + "" + TextFormatter::get_format(fg, bg);
        text_to_display += " " + entries[i] + " ";
        text_to_display += TextFormatter::get_format(bg, default_color);
        if (i < entries.size() - 1)
            text_to_display += "";
        else
            text_to_display += "";
    }
    return text_to_display;
}

string StatusBar::getFormattedFilePath(const string& delimiter, int max_folders = 0) {
    stringstream ss(file_path);
    string segment;
    vector<string> path_segments;

    while (getline(ss, segment, '\\')) {
        path_segments.push_back(segment);
    }
    if (max_folders > 0 && max_folders < path_segments.size()) {
        path_segments.erase(path_segments.begin(), path_segments.end() - max_folders);
    }
    string path_str = " ";
    for (size_t i = 0; i < path_segments.size(); ++i) {
        path_str += delimiter + path_segments[i];
    }
    return "\u001b]8;;" + file_path + "\u001b\\" + path_str + "\u001b]8;;\u001b\\";
}