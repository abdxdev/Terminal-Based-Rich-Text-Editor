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

enum eFormats {
	BOLD,
	ITALIC,
	UNDERLINE,
	STRIKETHROUGH
};

class Format {
public:
	eFormats formatName;
	char formattingChar;
	string start;
	string stop;
	Format(eFormats formatName, char formattingChar, string start, string stop) : formatName(formatName), formattingChar(formattingChar), start(start), stop(stop) {}
	Format() = default;
};

class TopBar {
public:
	const vector<Format> formats = {
		Format(eFormats::BOLD, 'B', TextFormatter::BOLD, TextFormatter::N_BOLD),
		Format(eFormats::ITALIC, 'I', TextFormatter::ITALIC, TextFormatter::N_ITALIC),
		Format(eFormats::UNDERLINE, 'U', TextFormatter::UNDERLINE, TextFormatter::N_UNDERLINE),
		Format(eFormats::STRIKETHROUGH, 'S', TextFormatter::STRIKETHROUGH, TextFormatter::N_STRIKETHROUGH),
	};

	vector<string> entries;
	map<string, int> entryPositions;
	pair<int, int> screenSize;
	int topBarLength;
	bool isAutoSuggestionOn;
	string foregroundColor;
	string backgroundColor;
	vector<eFormats> selectedFormats;

public:
	TopBar() = default;
	TopBar(vector<eFormats> selectedFormats, string foregroundColor, string backgroundColor, bool isAutoSuggestionOn, pair<int, int> screenSize);
	string makePill(string text, string pill_color, string pill_background, string foreground_color);
	void toggleFormat(eFormats selectedFormat);
	string getCurrentFormatting(eFormats format);
	string getCurrentFormattings();
	void toggleAutoSuggestion();
	void changeForegroundColor(string color);
	void changeBackgroundColor(string color);

	void updateScreenSize(pair<int, int> screenSize);
	string formatEntries();
	void display();
};
