#include "GLFWFramebuffer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

GLFWFramebuffer::GLFWFramebuffer(const int width, const int height)
    :Framebuffer(width, height)
{
    glfwInit();

    if( !glfwOpenWindow( width * 10, height * 10, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();
    }

    glfwSetWindowTitle("TextTracer");

    // Setup screen texture
    m_screenBuffer = new GLubyte[width * height * 4];

    glGenTextures(1, &m_screenTexture);
    glBindTexture(GL_TEXTURE_2D, m_screenTexture);

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
}

void GLFWFramebuffer::PaintCell(const int x, const int y, const glm::vec4& colour)
{
    int cellIdx = ((y * m_width) + x) * 4;
    m_screenBuffer[cellIdx + 0] = (int)(colour.r * 255.0f);
    m_screenBuffer[cellIdx + 1] = (int)(colour.g * 255.0f);
    m_screenBuffer[cellIdx + 2] = (int)(colour.b * 255.0f);
    m_screenBuffer[cellIdx + 3] = 255;

}

void GLFWFramebuffer::Draw()
{
    // Generate texture from screen buffer
    gluBuild2DMipmaps( GL_TEXTURE_2D, 4, m_width, m_height,
                   GL_RGBA, GL_UNSIGNED_BYTE, m_screenBuffer );

    // Draw to screen
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glBegin (GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);
    glEnd();
    glfwSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}
