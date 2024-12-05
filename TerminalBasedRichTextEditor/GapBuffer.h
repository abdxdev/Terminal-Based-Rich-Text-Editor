#pragma once

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
// #include <utility>

using namespace std;

class GapBuffer {
private:
    vector<char> buffer;
    int gapStart;
    int gapEnd;

	string clipboard;
	pair<int, int> selection = {-1, -1};
	stack<pair<int, string>> undoStack;
	stack<pair<int, string>> redoStack;

    void expandBuffer();
    void moveGap(int position);

public:
    GapBuffer(int initialSize = 10);

    void insert(char c);
    void insert(const string& text);
    void deleteChar();
    void deleteChars(int count);
    void moveCursor(int position);
    void moveCursorRelative(int offset);

    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
    int find(const string& text) const;
    void replace(const string& oldText, const string& newText);
    void copy(int start, int length);
    void paste();
    void selectText(int start, int end);
    void deleteSelection();

    void display() const;

    void undo();
    void redo();
};