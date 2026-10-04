#include "screen.hpp"
#include "utils.hpp"
#include <GLFW/glfw3.h>
#include <external/glad.h>

namespace bromine
{
Screen::Screen() : window(nullptr), raylibWindow(nullptr) {}

Screen::~Screen()
{
    if (window != nullptr)
    {
        glfwDestroyWindow(window);
    }
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

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    glfwMakeContextCurrent(raylibWindow);
}
} // namespace bromine
