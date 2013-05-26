#ifndef DEBUGBOX_H
#define DEBUGBOX_H

#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

struct DebugBox
{
    static void WriteMessage(const std::stringstream& message, const int line)
    {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO SBInfo;
        GetConsoleScreenBufferInfo(hConsole, &SBInfo);

        COORD prevCursorPosition = SBInfo.dwCursorPosition;
        int prevTextColour = SBInfo.wAttributes;
        SHORT offsetLine = line + 32;
        COORD newCursorPosition = {0, offsetLine};
        SetConsoleCursorPosition(hConsole, newCursorPosition);
        SetConsoleTextAttribute(hConsole, 8 * 16);
        std::cout << message.str();
        SetConsoleCursorPosition(hConsole, prevCursorPosition);
        SetConsoleTextAttribute(hConsole, prevTextColour);
#endif // _WIN32
    }
};

#endif // DEBUGBOX_H
