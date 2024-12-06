#include "DebugUtils.h"

void DebugUtils::displayDebugMessage(const string& message, const pair<int, int>& position) {
	cout << "\033[" << position.first << ";" << position.second << "H" << message;
}

int DebugUtils::checkKeys() {
	while (true) {
		int ch;
		while ((ch = _getch()) != 27) {
			printf("%d", ch);
			if (ch == 0 || ch == 224)
				printf(", %d", _getch());
			printf("\n");
		}
		printf("ESC %d\n", ch);
	}
}