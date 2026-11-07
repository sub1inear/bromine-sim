#pragma once
#include <chrono>
#include <raylib.h>

namespace bromine
{
class Window
{
public:
    Window();
    ~Window();
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    bool shouldClose();
    void waitForNextFrame();

private:
    std::chrono::steady_clock::duration framePeriod;
    std::chrono::steady_clock::time_point frameDeadline;
};
} // namespace bromine
