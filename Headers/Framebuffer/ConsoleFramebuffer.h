#ifndef CONSOLEFRAMEBUFFER_H
#define CONSOLEFRAMEBUFFER_H

#include <vector>

#include "Framebuffer.h"

class ConsoleFramebuffer : public Framebuffer
{
public:
    ConsoleFramebuffer(const int width, const int height);
    ~ConsoleFramebuffer();

    void Clear();
    void PaintCell(const int x, const int y, const glm::vec4& colour);
    void Draw();
    bool IsKeyDown(const char Key) const;

private:
    void setColour(const glm::vec4& colour);
#ifdef _WIN32
    int nearestWinConsoleColour(glm::vec4 colour);
#endif
    float colourDistance(const glm::vec4& c0, const glm::vec4& c1);

    std::vector<glm::vec4> m_colourPalette;
    std::vector<glm::vec4> m_buffer;
};

#endif // CONSOLEFRAMEBUFFER_H

