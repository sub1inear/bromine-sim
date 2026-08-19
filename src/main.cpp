#include "constants.hpp"
#include "renderer.hpp"
#include "sim.hpp"
#include "window.hpp"
#include <raylib.h>

int main()
{
    cobalt::Window window;

    SetTargetFPS(cobalt::fps);

    cobalt::Sim sim;
    cobalt::Renderer renderer;

    while (!window.shouldClose())
    {
        sim.update(GetFrameTime());
        renderer.draw(sim);
    }

    return 0;
}
