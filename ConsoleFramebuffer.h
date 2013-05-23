#ifndef CONSOLEFRAMEBUFFER_H
#define CONSOLEFRAMEBUFFER_H

#include <vector>

#include "Framebuffer.h"

class ConsoleFramebuffer : public Framebuffer
{
public:
    ConsoleFramebuffer(int width, int height);
    ~ConsoleFramebuffer();

    void Clear(glm::vec4 colour);
    void PaintCell(int x, int y, glm::vec4 colour);
    void Draw();

private:
    void setColour(glm::vec4 colour);
#ifdef _WIN32
    int nearestWinConsoleColour(glm::vec4 colour);
#endif
    float colourDistance(glm::vec4 c0, glm::vec4 c1);

    std::vector<glm::vec4> m_colourPalette;
    std::vector<glm::vec4> m_buffer;
};

#endif // CONSOLEFRAMEBUFFER_H

