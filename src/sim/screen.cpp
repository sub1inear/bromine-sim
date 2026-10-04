#include "screen.hpp"
#include "utils.hpp"
#include <GLFW/glfw3.h>
#include <external/glad.h>

namespace bromine
{
Screen::Screen() : window(nullptr), raylibWindow(nullptr), texture(0), fbo(0) {}

Screen::~Screen()
{
    glfwMakeContextCurrent(window);
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &fbo);
    glfwMakeContextCurrent(raylibWindow);
    glfwDestroyWindow(window);
}

void Screen::createWindow()
{
    raylibWindow = glfwGetCurrentContext();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(480, 240, "V5 Brain", nullptr, nullptr);

    if (window == nullptr)
    {
        error("failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 480, 240, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        error("failed to create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwMakeContextCurrent(raylibWindow);
}

void Screen::update()
{
    if (window == nullptr)
    {
        createWindow();
    }

    if (glfwWindowShouldClose(window))
    {
        glfwSetWindowShouldClose(window, GLFW_FALSE);
        glfwHideWindow(window);
    }
    glfwMakeContextCurrent(window);
    glfwPollEvents();

    if (dirty.exchange(false, std::memory_order_acquire))
    {
        for (size_t i = 0; i < buffer.size(); i++)
        {
            copyBuffer[i] = buffer[i].load(std::memory_order_relaxed);
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 480, 240, GL_BGRA,
                        GL_UNSIGNED_BYTE, copyBuffer.data());

        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, 480, 240, 0, 240, 480, 0, GL_COLOR_BUFFER_BIT,
                          GL_NEAREST);

        glfwSwapBuffers(window);
    }
    glfwMakeContextCurrent(raylibWindow);
}
} // namespace bromine
