#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>

#include "ColouredText.h"

class Framebuffer
{
public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    int Width() const { return m_width; }
    int Height() const { return m_height; }

    void Clear(Colour colour);
    void PaintCell(int x, int y, Colour colour);
    void Draw();

private:
    int m_width;
    int m_height;
    std::vector<Colour> m_buffer;
};

#endif // FRAMEBUFFER_H
