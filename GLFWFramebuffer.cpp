#include "GLFWFramebuffer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

GLFWFramebuffer::GLFWFramebuffer(const int width, const int height)
    :Framebuffer(width, height)
{
    glfwInit();

    GLFWvidmode vm;
    glfwGetDesktopMode(&vm);
    float ar = (float)width / (float)height;
    float windowWidth = vm.Width / 2;
    float windowHeight = (vm.Height / 2) * ar;

    if( !glfwOpenWindow( vm.Width / 2, (vm.Height / 2) * ar, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();
    }

    glfwSetWindowPos(vm.Width / 2 - windowWidth / 2, vm.Height / 2 - windowHeight / 2);

    glfwSetWindowTitle("TextTracer");

    // Setup screen texture
    m_screenBuffer = new GLubyte[width * height * 3];

    glGenTextures(1, &m_screenTexture);
    glBindTexture(GL_TEXTURE_2D, m_screenTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //Populate with blank cells to ensure sanity
    Clear();
}

GLFWFramebuffer::~GLFWFramebuffer()
{
    glfwTerminate();
    delete m_screenBuffer;
}

void GLFWFramebuffer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLFWFramebuffer::PaintCell(const int x, const int y, const glm::vec4& colour)
{
    int cellIdx = ((y * m_width) + x) * 3;
    m_screenBuffer[cellIdx + 0] = (int)(colour.r * 255.0f);
    m_screenBuffer[cellIdx + 1] = (int)(colour.g * 255.0f);
    m_screenBuffer[cellIdx + 2] = (int)(colour.b * 255.0f);
}

void GLFWFramebuffer::Draw()
{
    // Generate texture from screen buffer
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        3,
        m_width,
        m_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        m_screenBuffer
    );

    // Draw to screen
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
        glEnable(GL_TEXTURE_2D);
            glBegin (GL_QUADS);
                glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);
                glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);
            glEnd();
        glDisable(GL_TEXTURE_2D);
    glfwSwapBuffers();
}
