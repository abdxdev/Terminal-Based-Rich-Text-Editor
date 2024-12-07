// #include "DebugUtils.h"
// #include "Utils.h"
#include "Editor.h"
#include "AutoSuggestion.h"

using namespace std;

int main() {
	do {
		system("pause");
		system("cls");
		// cout << AutoSuggestion::getInput(4, { 1,1 }, 30);
	} while (1);
	//DebugUtils::checkKeys();
	Editor editor;
	editor.run();
	return 0;
}
