#ifndef GLFWFRAMEBUFFER_H
#define GLFWFRAMEBUFFER_H

#include <vector>

#include "Framebuffer.h"
#include <GLFW/glfw3.h>

class GLFWFramebuffer : public Framebuffer
{
public:
    GLFWFramebuffer(const int width, const int height);
    ~GLFWFramebuffer();

    void Clear();
    void PaintCell(const int x, const int y, const glm::vec4& colour);
    void Draw();
    bool IsKeyDown(const char Key) const;

private:
    GLubyte* m_screenBuffer;
    GLuint m_screenTexture;
    GLFWwindow* m_window;
};

#endif // GLFWFRAMEBUFFER_H

