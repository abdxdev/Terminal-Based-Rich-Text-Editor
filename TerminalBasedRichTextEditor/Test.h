#pragma once
#include <conio.h>
#include <stdio.h>

int CheckKeys() {
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