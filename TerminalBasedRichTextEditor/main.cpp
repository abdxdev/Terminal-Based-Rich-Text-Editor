// #include "DebugUtils.h"
// #include "Utils.h"
#include "Editor.h"

int main() {
    // DebugUtils::CheckKeys();
    cout << "Make terminal full screen and press enter";
    _getch();
    system("cls");
    //Utils::printWide(L"\x1b[38;2;12;33;47m\x1b[48;2;0;0;0m\x1b[38;2;38;189;187m\x1b[48;2;12;33;47m  \x1b[38;2;12;33;47m\x1b[48;2;0;0;0m\x1b[38;2;0;0;0m\x1b[48;2;38;189;187m\x1b[38;2;12;33;47m\x1b[48;2;38;189;187m Rich Text Editor \x1b[38;2;38;189;187m");


    Editor editor;
    editor.run();
    return 0;
}
