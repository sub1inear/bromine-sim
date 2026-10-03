#pragma once
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
};
} // namespace bromine
