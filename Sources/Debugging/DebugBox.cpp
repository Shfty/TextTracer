#include "DebugBox.h"

DebugBox& DebugBox::GetInstance()
{
    static DebugBox instance;

    return instance;
}

void DebugBox::WriteMessage(const int screenHeight, const int line)
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    GetConsoleScreenBufferInfo(hConsole, &SBInfo);

    COORD prevCursorPosition = SBInfo.dwCursorPosition;
    int prevTextColour = SBInfo.wAttributes;
    SHORT offsetLine = screenHeight + line;
    COORD newCursorPosition = {0, offsetLine};
    SetConsoleCursorPosition(hConsole, newCursorPosition);
    SetConsoleTextAttribute(hConsole, 8 * 16);
    std::cout << Message.str();
    SetConsoleCursorPosition(hConsole, prevCursorPosition);
    SetConsoleTextAttribute(hConsole, prevTextColour);
#else
    std::cout << Message.str() << "\n";
#endif // _WIN32

    //Clear the stringstream in prep for the next message
    Message.str(std::string());
}
