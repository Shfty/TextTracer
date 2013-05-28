#ifndef GLFWFRAMEBUFFER_H
#define GLFWFRAMEBUFFER_H

#include <vector>

#include "Framebuffer.h"
#include <GL/glfw.h>

class GLFWFramebuffer : public Framebuffer
{
public:
    GLFWFramebuffer(const int width, const int height);
    ~GLFWFramebuffer();

    void Clear();
    void PaintCell(const int x, const int y, const glm::vec4& colour);
    void Draw();

private:
    GLubyte* m_screenBuffer;
    GLuint m_screenTexture;
};

#endif // GLFWFRAMEBUFFER_H

