#pragma once

#include "GapBuffer.h"
#include <utility>
#include <iostream>

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
    void displayCurrentLine() const;

    int getCursorRow() const;
    int getCursorColumn() const;
    pair<int, int> getCursorPosition() const;

private:
    void splitLine();
    void mergeLine();
};
