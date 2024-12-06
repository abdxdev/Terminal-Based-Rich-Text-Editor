#include "GapBuffer.h"

class GapBufferLinkedList {
public:
    struct Line {
        GapBuffer gapBuffer;
        Line* prev;
        Line* next;

        Line(int capacity = 10) : gapBuffer(capacity), prev(nullptr), next(nullptr) {}
    };

    Line* head;
    Line* currentLine;
    int cursorRow;

public:
    GapBufferLinkedList() : head(new Line()), currentLine(head), cursorRow(0) {}

    ~GapBufferLinkedList() {
        while (head) {
            Line* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insert(char c) {
        if (c == '\n') {
            splitLine();
        } else {
            currentLine->gapBuffer.insert(c);
        }
    }

    void deleteCharLeft() {
        if (getCursorColumn() > 0) {
            currentLine->gapBuffer.deleteCharLeft();
        } else if (currentLine->prev) {
            mergeLine();
        }
    }

    void deleteCharRight() {
        if (getCursorColumn() < currentLine->gapBuffer.getLength()) {
            currentLine->gapBuffer.deleteCharRight();
        } else if (currentLine->next) {
            mergeLine();
        }
    }

    void moveCursorUp() {
        if (currentLine->prev) {
            currentLine = currentLine->prev;
            --cursorRow;
        }
    }

    void moveCursorDown() {
        if (currentLine->next) {
            currentLine = currentLine->next;
            ++cursorRow;
        }
    }

    void moveCursorLeft() {
        if (getCursorColumn() == 0 && currentLine->prev) {
            moveCursorUp();
        } else {
            currentLine->gapBuffer.moveCursorRelative(-1);
        }
    }

    void moveCursorRight() {
        if (getCursorColumn() == currentLine->gapBuffer.getLength() && currentLine->next) {
            moveCursorDown();
        } else {
            currentLine->gapBuffer.moveCursorRelative(1);
        }
    }

    void display() const {
        Line* temp = head;
        while (temp) {
            temp->gapBuffer.display();
            temp = temp->next;
        }
    }

    void displayCurrentLine() const {
        currentLine->gapBuffer.display();
    }

    int getCursorRow() const {
        return cursorRow;
    }

    int getCursorColumn() const {
        return currentLine->gapBuffer.getCursorPosition();
    }

    pair<int, int> getCursorPosition() const {
        return {getCursorRow(), getCursorColumn()};
    }

private:
    void splitLine() {
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

    void mergeLine() {
        Line* temp = currentLine;
        currentLine = currentLine->prev;
        currentLine->next = temp->next;
        if (temp->next) {
            temp->next->prev = currentLine;
        }
        delete temp;
        --cursorRow;
    }
};
