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

    string currentInsertBuffer;
    int visualCursorPosition;
    bool inSequence;
    
    pair<int, int> selection = {-1, -1};
    string clipboard;
     
    int insertStartPosition;
    bool isRedoOperation;
    stack<pair<int, string>> undoStack;
    stack<pair<int, string>> redoStack;

    void expandBuffer();
    void moveGap(int position);
    bool isInAnsiSequence(int pos) const;
    int getBufferPosition(int visualPos) const;
    int getVisualPosition(int bufferPos) const;
    int getAnsiSequenceLength(int start) const;

public:
    GapBuffer(int initialSize = 10);

    void insert(char c);
    void insert(const string& text);
    void deleteCharLeft();
    void deleteCharRight();
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

    void display(int from, int to) const;
    string getDebugText() const;
    string getLine() const;
    int getLength() const;
    // int getCursorPosition() const;
    int getBufferCursorPosition() const;
    int getVisualCursorPosition() const;

    void undo();
    void redo();
    void commitInsertOperation();

    string getTextAfterCursor() const;
    void deleteToEnd();
};
