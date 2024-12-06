#pragma once

#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

class DebugUtils {
public:
    static void displayDebugMessage(const string& message, const pair<int, int>& position);
    static int checkKeys();
};
