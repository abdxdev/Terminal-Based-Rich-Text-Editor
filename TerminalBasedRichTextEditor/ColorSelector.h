#include <iostream>
#include <vector>
#include <conio.h>
#include "TextFormatter.h"
#include "Cursor.h"

class ColorSelector {
private:
    int leftBound;
    int rightBound;
    int topBound;
    int bottomBound;
    int cursorX;
    int cursorY;

    string getColorAtPosition(int x, int y) {
        int width = rightBound - leftBound + 1;
        int height = bottomBound - topBound + 1;
        int h = ((x - leftBound) * 360) / width;
        int l = 100 - (((y - topBound) * 100) / height);
        return TextFormatter::hsl_to_ansi(h, 100, l, true);
    }

    void drawCell(int x, int y, bool isCursor) {
        Cursor::move_to({ y, x });
        string colorCode = getColorAtPosition(x, y);
        cout << TextFormatter::convert_to_bg(colorCode);
        cout << (isCursor ? TextFormatter::hex_to_ansi("#000000") + "O" : " ");
        cout << TextFormatter::RESET;
    }

public:
    ColorSelector(int left, int right, int top, int bottom)
        : leftBound(left), rightBound(right), topBound(top), bottomBound(bottom) {
        // Initialize cursor to top-left corner of the selection area
        cursorX = leftBound;
        cursorY = topBound;

        Cursor::hide();
        for (int y = topBound; y <= bottomBound; y++) {
            for (int x = leftBound; x <= rightBound; x++) {
                drawCell(x, y, x == cursorX && y == cursorY);
            }
        }
    }

    ~ColorSelector() {
        Cursor::show();
    }

    string getCurrentColor() {
        return getColorAtPosition(cursorX, cursorY);
    }

    bool handleInput() {
        int key = _getch();
        bool moved = false;
        int oldX = cursorX;
        int oldY = cursorY;

        if (key == 224) { // Arrow key prefix
            key = _getch();
            switch (key) {
            case 72: // Up
                if (cursorY > topBound) {
                    cursorY--;
                    moved = true;
                }
                break;
            case 80: // Down
                if (cursorY < bottomBound) {
                    cursorY++;
                    moved = true;
                }
                break;
            case 75: // Left
                if (cursorX > leftBound) {
                    cursorX--;
                    moved = true;
                }
                break;
            case 77: // Right
                if (cursorX < rightBound) {
                    cursorX++;
                    moved = true;
                }
                break;
            }
        }
        else if (key == 27 || key == 13) // ESC or Enter
            return false;

        if (moved) {
            drawCell(oldX, oldY, false);
            drawCell(cursorX, cursorY, true);
        }
        return true;
    }

    static string selectColor(pair<int, int> current_pos, int height, int width) {
        
        ColorSelector selector(current_pos.second, current_pos.second + width - 1, current_pos.first, current_pos.first + height - 1);
        while (selector.handleInput()) {}
        return selector.getCurrentColor();
    }
};