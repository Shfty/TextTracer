#include "GLFWFramebuffer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

GLFWFramebuffer::GLFWFramebuffer(const int width, const int height)
    :Framebuffer(width, height)
{
    glfwInit();

    // Calculate window size and attempt to open
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vm = glfwGetVideoMode(monitor);
    float ar = (float)width / (float)height;
    float windowWidth = vm->width / 2;
    float windowHeight = vm->height / 2 * ar;

    m_window = glfwCreateWindow( windowWidth, windowHeight, "TextTracer", NULL, NULL );
    if( !m_window )
    {
        glfwTerminate();
    }

    // Position centrally
    glfwSetWindowPos(m_window, vm->width / 2 - windowWidth / 2, vm->height / 2 - windowHeight / 2);

    // Set the active context
    glfwMakeContextCurrent(m_window);

    // Init screen texture
    m_screenBuffer = new GLubyte[width * height * 3];
    glGenTextures(1, &m_screenTexture);
    glBindTexture(GL_TEXTURE_2D, m_screenTexture);

    // Setup texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //Clear screen to ensure sanity
    Clear();

    // Finally, show the window
    glfwShowWindow(m_window);
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
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool GLFWFramebuffer::IsKeyDown(const char key) const
{
#ifdef _WIN32
    return GetAsyncKeyState(key);
#else
    int state = glfwGetKey(m_window, key);
    return state == GLFW_PRESS;
#endif
}
