#include "GapBufferLinkedList.h"

GapBufferLinkedList::GapBufferLinkedList() : head(new Line()),
                                             currentLine(head),
                                             cursorRow(0) {}

GapBufferLinkedList::~GapBufferLinkedList() {
    while (head) {
        Line* temp = head;
        head = head->next;
        delete temp;
    }
}

void GapBufferLinkedList::insert(char c) {
    if (c == '\n') {
        splitLine();
    } else {
        currentLine->gapBuffer.insert(c);
    }
}

void GapBufferLinkedList::insert(const string& s) {
    for (char c : s) {
        insert(c);
    }
}

void GapBufferLinkedList::deleteCharLeft() {
    if (currentLine->gapBuffer.getVisualCursorPosition() > 0) {
        currentLine->gapBuffer.deleteCharLeft();
    } else if (currentLine->prev) {
        int prevLineLength = currentLine->prev->gapBuffer.getLength();
        mergeLine();
        currentLine->gapBuffer.moveCursor(prevLineLength);
    }
}

void GapBufferLinkedList::deleteCharRight() {
    if (currentLine->gapBuffer.getVisualCursorPosition() < currentLine->gapBuffer.getLength()) {
        currentLine->gapBuffer.deleteCharRight();
    } else if (currentLine->next) {
        currentLine->gapBuffer.deleteCharRight();
        mergeLine();
    }
}

void GapBufferLinkedList::undo() {
    currentLine->gapBuffer.undo();
}

void GapBufferLinkedList::redo() {
    currentLine->gapBuffer.redo();
}

void GapBufferLinkedList::moveCursorUp() {
    if (!currentLine->prev)
        return;
    int currentVisualColumn = currentLine->gapBuffer.getVisualCursorPosition();
    currentLine = currentLine->prev;
    currentLine->gapBuffer.moveCursor(
        min(currentVisualColumn,
            currentLine->gapBuffer.getLength()));
    --cursorRow;
}

void GapBufferLinkedList::moveCursorDown() {
    if (!currentLine->next)
        return;
    int currentVisualColumn = currentLine->gapBuffer.getVisualCursorPosition();
    currentLine = currentLine->next;
    currentLine->gapBuffer.moveCursor(
        min(currentVisualColumn,
            currentLine->gapBuffer.getLength()));
    ++cursorRow;
}

void GapBufferLinkedList::moveCursorLeft() {
    if (currentLine->gapBuffer.getVisualCursorPosition() == 0 && currentLine->prev) {
        currentLine = currentLine->prev;
        currentLine->gapBuffer.moveCursor(currentLine->gapBuffer.getLength());
        --cursorRow;
    } else {
        currentLine->gapBuffer.moveCursorRelative(-1);
    }
}

void GapBufferLinkedList::moveCursorRight() {
    if (currentLine->gapBuffer.getVisualCursorPosition() == currentLine->gapBuffer.getLength() && currentLine->next) {
        currentLine = currentLine->next;
        currentLine->gapBuffer.moveCursor(0);
        ++cursorRow;
    } else {
        currentLine->gapBuffer.moveCursorRelative(1);
    }
}

void GapBufferLinkedList::moveCursor(int row, int column) {
    Line* temp = head;
    for (int i = 0; i < row; i++) {
        if (!temp->next)
            break;
        temp = temp->next;
    }
    currentLine = temp;
    currentLine->gapBuffer.moveCursor(column);
    cursorRow = row;
}

void GapBufferLinkedList::display() const {
    Line* temp = head;
    while (temp) {
        temp->gapBuffer.display(-1, -1);
        cout << '\n';
        temp = temp->next;
    }
}

void GapBufferLinkedList::displayCurrentLine(int from, int to) const {
    currentLine->gapBuffer.display(from, to);
}

string GapBufferLinkedList::getCurrentDebugLine() const {
    return currentLine->gapBuffer.getDebugText();
}

int GapBufferLinkedList::getCursorRow() const {
    return cursorRow;
}

int GapBufferLinkedList::getCursorColumn() const {
    return currentLine->gapBuffer.getVisualCursorPosition();
}

pair<int, int> GapBufferLinkedList::getCursorPosition() const {
    return {cursorRow, currentLine->gapBuffer.getVisualCursorPosition()};
}

int GapBufferLinkedList::getLinesCount() const {
    int count = 0;
    Line* temp = head;
    while (temp) {
        ++count;
        temp = temp->next;
    }
    return count;
}

vector<string> GapBufferLinkedList::getLines(int from, int to) const {
    vector<string> lines;
    Line* temp = head;
    for (int i = 0; i < from; ++i) {
        temp = temp->next;
    }
    for (int i = from; i <= to; ++i) {
        lines.push_back(temp->gapBuffer.getLine());
        temp = temp->next;
    }
    return lines;
}

void GapBufferLinkedList::save(string file_path) {
    ofstream file(file_path);
    Line* temp = head;
    while (temp) {
        file << temp->gapBuffer.getLine() << '\n';
        temp = temp->next;
    }
    file.close();
}

void GapBufferLinkedList::splitLine() {
    int cursorPos = currentLine->gapBuffer.getBufferCursorPosition();
    string rightContent = currentLine->gapBuffer.getTextAfterCursor();
    currentLine->gapBuffer.deleteToEnd();

    Line* newLine = new Line();
    newLine->prev = currentLine;
    newLine->next = currentLine->next;
    if (currentLine->next) {
        currentLine->next->prev = newLine;
    }
    currentLine->next = newLine;
    newLine->gapBuffer.insert(rightContent);
    currentLine = newLine;
    currentLine->gapBuffer.moveCursor(0);
    ++cursorRow;
}

void GapBufferLinkedList::mergeLine() {
    Line* temp = currentLine;
    currentLine = currentLine->prev;
    currentLine->gapBuffer.insert(temp->gapBuffer.getTextAfterCursor());
    currentLine->next = temp->next;
    if (temp->next) {
        temp->next->prev = currentLine;
    }
    delete temp;
    --cursorRow;
}
