#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glm/glm.hpp>
#include <vector>

class Framebuffer
{
public:
    Framebuffer(int width, int height) { m_width = width; m_height = height; };
    ~Framebuffer() {};

    int Width() const { return m_width; }
    int Height() const { return m_height; }

    virtual void Clear(glm::vec4 colour) = 0;
    virtual void PaintCell(int x, int y, glm::vec4 colour) = 0;
    virtual void Draw() = 0;

protected:
    int m_width;
    int m_height;
};

#endif // FRAMEBUFFER_H
