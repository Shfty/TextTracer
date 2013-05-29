#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

struct Keyboard
{
    static bool IsKeyDown(const char key);
};

#endif // KEYBOARD_H
