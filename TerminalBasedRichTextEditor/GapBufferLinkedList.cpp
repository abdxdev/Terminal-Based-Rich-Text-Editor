#include "GapBufferLinkedList.h"

GapBufferLinkedList::Line::Line(int capacity)
    : gapBuffer(capacity), prev(nullptr), next(nullptr) {}

GapBufferLinkedList::GapBufferLinkedList()
    : head(new Line()), currentLine(head), cursorRow(0) {}

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

void GapBufferLinkedList::deleteCharLeft() {
    if (getCursorColumn() > 0) {
        currentLine->gapBuffer.deleteCharLeft();
    } else if (currentLine->prev) {
        mergeLine();
    }
}

void GapBufferLinkedList::deleteCharRight() {
    if (getCursorColumn() < currentLine->gapBuffer.getLength()) {
        currentLine->gapBuffer.deleteCharRight();
    } else if (currentLine->next) {
        mergeLine();
    }
}

void GapBufferLinkedList::moveCursorUp() {
    if (currentLine->prev) {
        currentLine = currentLine->prev;
        --cursorRow;
    }
}

void GapBufferLinkedList::moveCursorDown() {
    if (currentLine->next) {
        currentLine = currentLine->next;
        ++cursorRow;
    }
}

void GapBufferLinkedList::moveCursorLeft() {
    if (getCursorColumn() == 0 && currentLine->prev) {
        moveCursorUp();
    } else {
        currentLine->gapBuffer.moveCursorRelative(-1);
    }
}

void GapBufferLinkedList::moveCursorRight() {
    if (getCursorColumn() == currentLine->gapBuffer.getLength() && currentLine->next) {
        moveCursorDown();
    } else {
        currentLine->gapBuffer.moveCursorRelative(1);
    }
}

void GapBufferLinkedList::display() const {
    Line* temp = head;
    while (temp) {
        temp->gapBuffer.display();
        temp = temp->next;
    }
}

void GapBufferLinkedList::displayCurrentLine() const {
    currentLine->gapBuffer.display();
}

int GapBufferLinkedList::getCursorRow() const {
    return cursorRow;
}

int GapBufferLinkedList::getCursorColumn() const {
    return currentLine->gapBuffer.getCursorPosition();
}

pair<int, int> GapBufferLinkedList::getCursorPosition() const {
    return {getCursorRow(), getCursorColumn()};
}

void GapBufferLinkedList::splitLine() {
    Line* newLine = new Line();
    newLine->prev = currentLine;
    newLine->next = currentLine->next;
    if (currentLine->next) {
        currentLine->next->prev = newLine;
    }
    currentLine->next = newLine;
    currentLine = newLine;
    ++cursorRow;
}

void GapBufferLinkedList::mergeLine() {
    Line* temp = currentLine;
    currentLine = currentLine->prev;
    currentLine->next = temp->next;
    if (temp->next) {
        temp->next->prev = currentLine;
    }
    delete temp;
    --cursorRow;
}
