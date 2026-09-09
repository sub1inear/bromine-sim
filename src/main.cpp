#include "constants.hpp"
#include "renderer.hpp"
#include "sim.hpp"
#include "window.hpp"
#include <raylib.h>

int main()
{
    bromine::Window window;

    SetTargetFPS(bromine::fps);

    bromine::Sim sim;
    bromine::Renderer renderer(sim.worldId);

    while (!window.shouldClose())
    {
        sim.update(GetFrameTime());
        renderer.draw(sim);
    }

    return 0;
}
