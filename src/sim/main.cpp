#include "constants.hpp"
#include "pros/system/startup.h"
#include "renderer.hpp"
#include "sim.hpp"
#include "window.hpp"
#include <cstdint>
#include <raylib.h>
#include <thread>

int main()
{
    // Boot the PROS kernel with startup_main().
    // This will never return and Raylib requires the main thread,
    // so we run startup_main() in a detached thread.
    std::thread(startup_main).detach();

    {
        SetTraceLogLevel(LOG_NONE);
        SetTargetFPS(bromine::fps);

        bromine::Window window;

        bromine::Sim &sim = bromine::Sim::getInstance();
        bromine::Renderer renderer(sim.worldId);

        while (!window.shouldClose())
        {
            sim.update(GetFrameTime());
            renderer.draw(sim);
        }
    }

    // HACK: Exit the program without destructing to avoid PROS accessing the
    // simulator's memory after it has been freed.
    // This can be fixed with complicated lock systems, but letting the OS clean
    // up is far easier. Object destructors should still be kept up to date for
    // testing.
    std::quick_exit(0);
}
