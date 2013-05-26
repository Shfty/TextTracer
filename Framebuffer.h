#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glm/glm.hpp>
#include <vector>

class Framebuffer
{
public:
    Framebuffer(const int width, const int height) { m_width = width; m_height = height; };
    virtual ~Framebuffer() {};

    int Width() const { return m_width; }
    int Height() const { return m_height; }

    virtual void Clear(const glm::vec4& colour) = 0;
    virtual void PaintCell(const int x, const int y, const glm::vec4& colour) = 0;
    virtual void Draw() = 0;

protected:
    int m_width;
    int m_height;
};

#endif // FRAMEBUFFER_H
