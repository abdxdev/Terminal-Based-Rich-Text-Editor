#include "TopBar.h"

TopBar::TopBar(vector<eFormats> selectedFormats, string foregroundColor, string backgroundColor, bool isAutoSuggestionOn, pair<int, int> screenSize, int cursorStyle) {
    this->selectedFormats = selectedFormats;
    this->foregroundColor = foregroundColor;
    this->backgroundColor = backgroundColor;
    this->isAutoSuggestionOn = isAutoSuggestionOn;
    this->screenSize = screenSize;
    this->topBarLength = 0;
    this->cursorStyle = cursorStyle;
}
void TopBar::toggleFormat(eFormats format) {
    if (find(selectedFormats.begin(), selectedFormats.end(), format) != selectedFormats.end()) {
        selectedFormats.erase(remove(selectedFormats.begin(), selectedFormats.end(), format), selectedFormats.end());
    } else {
        selectedFormats.push_back(format);
    }
}

string TopBar::getCurrentFormatting(eFormats format) {
    for (auto f : formats) {
        if (f.formatName == format) {
            if (find(selectedFormats.begin(), selectedFormats.end(), format) != selectedFormats.end())
                return f.start;
        }
    }
    return formats[format].stop;
}

string TopBar::getCurrentFormattings() {
    string formattings = "";
    for (auto format : formats) {
        if (find(selectedFormats.begin(), selectedFormats.end(), format.formatName) != selectedFormats.end())
            formattings += format.start;
    }
    return formattings;
}

void TopBar::toggleAutoSuggestion() {
    isAutoSuggestionOn = isAutoSuggestionOn ? 0 : 1;
}

void TopBar::changeForegroundColor(string color) {
    this->foregroundColor = color;
}

void TopBar::changeBackgroundColor(string color) {
    this->backgroundColor = color;
}

void TopBar::toggleCursorStyle() {
    cursorStyle = (cursorStyle + 1) % 3;
}

void TopBar::updateScreenSize(pair<int, int> screen_size) {
    this->screenSize = screen_size;
    this->topBarLength = 0;
}

string TopBar::formatEntries() {
    string pill = "#123044";
    string bg = "#0c212f";
    string fg = "#26bdbb";
    string purple = "#7621de";
    string default_color = "#000000";
    string white = "#ffffff";
    entries = {
        "Foreground F1: " + TextFormatter::get_format(foregroundColor) + "",
        "Background F2: " + TextFormatter::get_format(backgroundColor) + "",
        "Suggestions F3: " + TextFormatter::get_format(purple) + ((isAutoSuggestionOn) ? "On" : "Off"),
        "Refresh F4",
        "Change Cursor F5",
    };

    string format_string = "Format: ";
    for (auto format : formats) {
        if (find(selectedFormats.begin(), selectedFormats.end(), format.formatName) != selectedFormats.end())
            format_string += TextFormatter::get_format(white, purple);
        else
            format_string += TextFormatter::get_format(purple, pill);
        format_string += " " + format.start + format.formattingChar + format.stop + " ";
    }
    entries.push_back(format_string);

    string formatted_entries = "";
    for (auto entry : entries) {
        formatted_entries += makePill(entry, pill, bg, fg);
    }
    return makePill(formatted_entries, bg, default_color, "");
}

string TopBar::makePill(string text, string pill_color, string pill_background, string foreground_color = "") {
    string left = TextFormatter::get_format(pill_color, pill_background) + "";
    string content = TextFormatter::get_format(foreground_color, pill_color) + " " + text + TextFormatter::get_format(foreground_color, pill_color) + " ";
    string right = TextFormatter::get_format(pill_color, pill_background) + "";

    return " " + left + content + right + " ";
}

void TopBar::display() {
    string formatted_entries = formatEntries();
    if (!topBarLength) {
        pair<int, int> position_old = Cursor::get_position();
        topBarLength = position_old.second;

        Utils::printWide(Utils::stows(formatted_entries));

        pair<int, int> position_new = Cursor::get_position();
        topBarLength = position_new.second - position_old.second;
        Cursor::clear_line();
        Cursor::move_to_start_of_line();
    }
    // cout << setw((screenSize.second - topBarLength) / 2) << ' ';
    Cursor::move_right((screenSize.second - topBarLength) / 2);
    Utils::printWide(Utils::stows(formatted_entries));
    cout << TextFormatter::RESET;
    Cursor::clear_line_from_cursor();
}