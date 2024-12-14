#pragma once

#include "GapBuffer.h"
#include <iostream>
#include <utility>

using namespace std;

class GapBufferLinkedList {
private:
    struct Line {
        GapBuffer gapBuffer;
        Line* prev;
        Line* next;
        explicit Line(int capacity = 10) : gapBuffer(capacity), prev(nullptr), next(nullptr) {}
    };

    Line* head;
    Line* currentLine;
    int cursorRow;

private:
    void splitLine();
    void mergeLine();

public:
    GapBufferLinkedList();
    ~GapBufferLinkedList();

    void insert(char c);
    void insert(const string& s);
    void deleteCharLeft();
    void deleteCharRight();

    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursor(int x, int y);

    void display() const;
    void displayCurrentLine(int from = 0, int to = -1) const;
    string getCurrentDebugLine() const;

    int getCursorRow() const;
    int getCursorColumn() const;
    int getLinesCount() const;
    pair<int, int> getCursorPosition() const;
    vector<string> getLines(int from, int to) const;
};
