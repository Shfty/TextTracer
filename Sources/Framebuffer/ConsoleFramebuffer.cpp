#include "ConsoleFramebuffer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

#include "TextTracer.h"

ConsoleFramebuffer::ConsoleFramebuffer(const int width, const int height)
    :Framebuffer(width, height)
{
    // Setup console window size
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SHORT cWidth = width;
    SHORT cHeight = width + TextTracer::HUD_HEIGHT;
    SMALL_RECT windowSize = {0, 0, cWidth, cHeight};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    SetConsoleTitle("Text Tracer");
#endif // _WIN32

    #ifdef _WIN32
    // Dark Colours
    m_colourPalette.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)); // Black
    m_colourPalette.push_back(glm::vec4(0.0f, 0.0f, 0.5f, 0.0f)); // Blue
    m_colourPalette.push_back(glm::vec4(0.0f, 0.5f, 0.0f, 0.0f)); // Green
    m_colourPalette.push_back(glm::vec4(0.0f, 0.5f, 0.5f, 0.0f)); // Cyan
    m_colourPalette.push_back(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f)); // Red
    m_colourPalette.push_back(glm::vec4(0.5f, 0.0f, 0.5f, 0.0f)); // Pink
    m_colourPalette.push_back(glm::vec4(0.5f, 0.5f, 0.0f, 0.0f)); // Yellow
    m_colourPalette.push_back(glm::vec4(0.75f, 0.75f, 0.75f, 0.0f)); // White
    // Bright Colours
    m_colourPalette.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 0.0f)); // Black
    m_colourPalette.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)); // Blue
    m_colourPalette.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)); // Green
    m_colourPalette.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 0.0f)); // Cyan
    m_colourPalette.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)); // Red
    m_colourPalette.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f)); // Pink
    m_colourPalette.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f)); // Yellow
    m_colourPalette.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)); // White
    #endif // _WIN32

    //Populate with blank cells to ensure sanity
    Clear();
}

ConsoleFramebuffer::~ConsoleFramebuffer()
{
}

void ConsoleFramebuffer::Clear()
{
    // Reset cursor position
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};

    SetConsoleCursorPosition(hConsole, coord);
#endif // _WIN32

    // Fill with clear colour
    m_buffer.assign(m_width * m_height, glm::vec4());
}

void ConsoleFramebuffer::PaintCell(const int x, const int y, const glm::vec4& colour)
{
    m_buffer[m_width * y + x] = colour;
    /*
    if(colour.a >= 1.0f)
    {
        m_buffer[m_width * y + x] = colour;
    }
    else
    {
        m_buffer[m_width * y + x] += (colour * colour.a);
    }
    */
}

void ConsoleFramebuffer::Draw()
{
    for(int i = 0; i < m_width * m_height; i++)
    {
        setColour(m_buffer[i]);

        std::cout << ' ';
        if(i % m_width == m_width - 1)
        {
            std::cout << std::endl;
        }
    }
}

void ConsoleFramebuffer::setColour(const glm::vec4& colour)
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, nearestWinConsoleColour(colour));
#endif // _WIN32
}

#ifdef _WIN32
int ConsoleFramebuffer::nearestWinConsoleColour(glm::vec4 colour)
{
    float currentDistance = 2.0f;
    int nearestColour = 0;

    for(uint16_t i = 0; i < m_colourPalette.size(); i++)
    {
        float distance = colourDistance(colour, m_colourPalette[i]);
        if(distance < currentDistance)
        {
            currentDistance = distance;
            nearestColour = i;
        }
    }

    return nearestColour * 16;
}
#endif // _WIN32

float ConsoleFramebuffer::colourDistance(const glm::vec4& c0, const glm::vec4& c1)
{
    float distance = 0.0f;

    distance += glm::abs(c0.r - c1.r);
    distance += glm::abs(c0.g - c1.g);
    distance += glm::abs(c0.b - c1.b);

    if(distance == 0)
    {
        return 0;
    }
    else
    {
        return distance / 3;
    }
}

bool ConsoleFramebuffer::IsKeyDown(const char key) const
{
#ifdef _WIN32
    return GetAsyncKeyState(key);
#else
    return false;
#endif
}
