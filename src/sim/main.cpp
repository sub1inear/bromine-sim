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

    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(bromine::fps);

    bromine::Window window;

    // Support V5 controllers with a custom mapping.
    // TODO: Test on Windows and Linux (confirmed on MacOS).
    // HACK: This should be in the bromine::Input class
    // but it must be initialized following the creation of the window
    // so it is here. Ideally this will be moved out somewhere.
    SetGamepadMappings(
        "03000000882800000305000000000000,VEX V5 Controller,"
        "a:b0,b:b1,x:b2,y:b3,leftshoulder:b4,rightshoulder:b5,lefttrigger:b6,"
        "righttrigger:b7,"
        "dpup:h0.1,dpright:h0.2,dpdown:h0.4,dpleft:h0.8,"
        "leftx:a0,lefty:a1,rightx:a2,righty:a3,platform:Windows,\n"
        "03000000882800000305000000010000,VEX V5 Controller,"
        "a:b0,b:b1,x:b2,y:b3,leftshoulder:b4,rightshoulder:b5,lefttrigger:b6,"
        "righttrigger:b7,"
        "dpup:h0.1,dpright:h0.2,dpdown:h0.4,dpleft:h0.8,"
        "leftx:a0,lefty:a1,rightx:a2,righty:a3,platform:Mac OS X,\n"
        "03000000882800000305000000010000,VEX V5 Controller,"
        "a:b0,b:b1,x:b2,y:b3,leftshoulder:b4,rightshoulder:b5,lefttrigger:b6,"
        "righttrigger:b7,"
        "dpup:h0.1,dpright:h0.2,dpdown:h0.4,dpleft:h0.8,"
        "leftx:a0,lefty:a1,rightx:a2,righty:a3,platform:Linux,\n");

    bromine::Sim &sim = bromine::Sim::getInstance();
    bromine::Renderer renderer(sim.worldId);

    // Boot the PROS kernel with startup_main().
    // This will never return and Raylib requires the main thread,
    // so we run startup_main() in a detached thread.
    std::thread(startup_main).detach();

    while (!window.shouldClose())
    {
        sim.update();
        renderer.draw(sim);
    }

    // HACK: Exit the program without destructing to avoid PROS accessing the
    // simulator's memory after it has been freed.
    // This can be fixed with complicated lock systems, but letting the OS clean
    // up is far easier. Object destructors should still be kept up to date for
    // testing.
    std::quick_exit(0);
}
