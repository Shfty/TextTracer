#ifndef DEBUGBOX_H
#define DEBUGBOX_H

#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

class TextTracer;

class DebugBox
{
public:
    static DebugBox& GetInstance();

    void WriteMessage(const int screenHeight, const int line);

    std::stringstream Message;

private:
    DebugBox() {};

    DebugBox(DebugBox const&);
    void operator=(DebugBox const&);
};

#endif // DEBUGBOX_H
