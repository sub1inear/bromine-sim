
#include "window.hpp"
#include <algorithm>
#include <raylib.h>

namespace bromine
{
Window::Window()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "");

    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    CloseWindow();

    int monitorSize = std::min(monitorWidth, monitorHeight);
    int windowSize = (int)((float)monitorSize * 0.8f);

    InitWindow(windowSize, windowSize, "BromineSim");
}

Window::~Window() { CloseWindow(); }

bool Window::shouldClose() { return WindowShouldClose(); }
} // namespace bromine
