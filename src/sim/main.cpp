#include "constants.hpp"
#include "renderer.hpp"
#include "sim.hpp"
#include "window.hpp"
#include <raylib.h>

int main()
{
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(bromine::fps);

    bromine::Window window;

    bromine::Sim &sim = bromine::getSim();
    bromine::Renderer renderer(sim.worldId);

    while (!window.shouldClose())
    {
        sim.update(GetFrameTime());
        renderer.draw(sim);
    }

    return 0;
}
