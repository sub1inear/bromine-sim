#include "window.hpp"
#include <algorithm>
#include <raylib.h>
#include <thread>

namespace bromine
{
Window::Window()
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(0, 0, "");

    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    CloseWindow();

    int monitorSize = std::min(monitorWidth, monitorHeight);
    int windowSize = (int)((float)monitorSize * 0.8f);

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(windowSize, windowSize, "Bromine Sim");
    ClearWindowState(FLAG_WINDOW_HIDDEN);

    int refreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    if (refreshRate <= 0)
    {
        refreshRate = 60;
    }
    framePeriod = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
        std::chrono::duration<double>(1.0 / refreshRate));
    frameDeadline = std::chrono::steady_clock::now();
}

Window::~Window() { CloseWindow(); }

bool Window::shouldClose() { return WindowShouldClose(); }

void Window::waitForNextFrame()
{
    frameDeadline += framePeriod;
    auto now = std::chrono::steady_clock::now();
    if (now - frameDeadline > framePeriod)
    {
        frameDeadline = now;
        return;
    }
    std::this_thread::sleep_until(frameDeadline);
}
} // namespace bromine
