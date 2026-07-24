
#include "window.hpp"
#include <algorithm>
#include <raylib.h>

namespace cobalt
{
Window::Window()
{
    InitWindow(0, 0, "");

    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    CloseWindow();

    int monitorSize = std::min(monitorWidth, monitorHeight);
    int windowSize = (int)((float)monitorSize * 0.8f);

    InitWindow(windowSize, windowSize, "CobaltSim");
}

Window::~Window() { CloseWindow(); }

bool Window::shouldClose() { return WindowShouldClose(); }
} // namespace cobalt
