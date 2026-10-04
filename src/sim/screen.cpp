#include "screen.hpp"
#include <GLFW/glfw3.h>
#include <external/glad.h>

namespace bromine
{
Screen::Screen() : window(nullptr), raylibWindow(nullptr), windowCreated(false)
{
}

Screen::~Screen()
{
    if (window != nullptr)
    {
        glfwDestroyWindow(window);
    }
}

void Screen::createWindow()
{
    windowCreated = true;
    raylibWindow = glfwGetCurrentContext();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(480 * 2, 240 * 2, "V5 Brain", nullptr, nullptr);
    glfwDefaultWindowHints();

    if (window == nullptr)
    {
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwMakeContextCurrent(raylibWindow);
}

void Screen::update()
{
    if (!windowCreated)
    {
        createWindow();
    }

    if (window == nullptr)
    {
        return;
    }

    if (glfwWindowShouldClose(window))
    {
        glfwSetWindowShouldClose(window, GLFW_FALSE);
        glfwHideWindow(window);
    }

    glfwMakeContextCurrent(window);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    glfwMakeContextCurrent(raylibWindow);
}
} // namespace bromine
