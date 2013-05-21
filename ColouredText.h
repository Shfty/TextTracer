#ifndef COLOUREDTEXT_H
#define COLOUREDTEXT_H

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Colour.h"

struct ColouredText
{
    static void SetColour(Colour colour, bool bright)
    {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int c = (int)colour;
        SetConsoleTextAttribute(hConsole, (c + (bright ? 8 : 0)) * 16);
#endif // _WIN32
    }
};

#endif // COLOUREDTEXT_H
