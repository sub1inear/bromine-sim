#pragma once
#include <GLFW/glfw3.h>
#include <array>
#include <atomic>
#include <cstdint>

struct GLFWwindow;
typedef unsigned int GLuint;

namespace bromine
{
class Screen
{
public:
    Screen();
    ~Screen();
    Screen(const Screen &) = delete;
    Screen &operator=(const Screen &) = delete;
    void update();

    std::array<std::atomic<uint32_t>, 480 * 240> buffer;
    std::atomic<bool> dirty;

    GLFWwindow *window;

private:
    GLFWwindow *raylibWindow;
    std::array<uint32_t, 480 * 240> copyBuffer;
    GLuint fbo;
    GLuint texture;
    void createWindow();
};
} // namespace bromine
