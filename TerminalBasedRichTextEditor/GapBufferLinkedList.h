#pragma once

#include "GapBuffer.h"
#include <iostream>
#include <utility>

using namespace std;

class GapBufferLinkedList {
public:
    struct Line {
        GapBuffer gapBuffer;
        Line* prev;
        Line* next;

        Line(int capacity = 10);
    };

private:
    Line* head;
    Line* currentLine;
    int cursorRow;
    // void splitLineContent();

public:
    GapBufferLinkedList();
    ~GapBufferLinkedList();

    void insert(char c);
    void deleteCharLeft();
    void deleteCharRight();

    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();

    void display() const;
    void displayCurrentLine(int from, int to) const;

    int getCursorRow() const;
    int getCursorColumn() const;
    int getLinesCount() const;
    pair<int, int> getCursorPosition() const;
    vector<string> getLines(int from, int to) const;

private:
    void splitLine();
    void mergeLine();
};
