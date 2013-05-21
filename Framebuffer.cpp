#include "Framebuffer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Colour.h"
#include "ColouredText.h"

Framebuffer::Framebuffer(int width, int height)
    :m_width(width),
    m_height(height)
{
    //Populate with blank cells to ensure sanity
    Clear(Colours::Black);
}

Framebuffer::~Framebuffer()
{
}

void Framebuffer::Clear(Colour colour)
{
    ColouredText::SetColour(colour, false);

#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};

    SetConsoleCursorPosition(hConsole, coord);
#endif // _WIN32

    m_buffer.assign(m_width * m_height, colour);
}

void Framebuffer::PaintCell(int x, int y, Colour colour)
{
    m_buffer[m_width * y + x] = colour;
}

void Framebuffer::Draw()
{
    for(int i = 0; i < m_width * m_height; i++)
    {
        ColouredText::SetColour(m_buffer[i], false);
        std::cout << ' ';
        if(i % m_width == m_width - 1)
        {
            std::cout << std::endl;
        }
    }
}
