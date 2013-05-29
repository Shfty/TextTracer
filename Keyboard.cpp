#include "Keyboard.h"

bool Keyboard::IsKeyDown(const char key)
{
#ifdef _WIN32
    return GetAsyncKeyState(key);
#endif
}
