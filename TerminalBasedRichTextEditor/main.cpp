// #include "DebugUtils.h"
// #include "Utils.h"
#include "Editor.h"

int main() {
    // DebugUtils::CheckKeys();
    cout << "Make terminal full screen and press enter";
    _getch();
    system("cls");
    Editor editor;
    editor.run();
    return 0;
}
