#pragma once

struct GLFWwindow;

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

private:
    GLFWwindow *window;
    GLFWwindow *raylibWindow;
    void createWindow();
};
} // namespace bromine
