#include "sim.hpp"
#include "constants.hpp"
#include "field.hpp"
#include <raylib.h>

namespace bromine
{
constexpr float maxDt = 0.25;
constexpr float fixedDt = 1.0f / fps;
constexpr uint32_t maxStepsPerFrame = 5;

static b2WorldId createWorld()
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    return b2CreateWorld(&worldDef);
}

// C++ doesn't allow field constructors to run following code in the
// encompassing class' constructor (to get worldId) before so we approximate
// this via a static function.
Sim::Sim() : worldId(createWorld()), robot(worldId), field(worldId), accum(0.0f)
{
}

Sim::~Sim()
{
    b2DestroyWorld(worldId); // Destroys inner bodies/shapes too.
}

void Sim::update()
{
    // See https://gafferongames.com/post/fix_your_timestep/.
    // This version doesn't bother to do a blended render.

    float dt = GetFrameTime();

    // Clamp the delta time so large freezes (e.g. dragging the window) doesn't
    // cause slowdowns.
    if (dt > maxDt)
    {
        dt = maxDt;
    }

    accum += dt;

    uint32_t steps = 0;
    while (accum >= fixedDt && steps < maxStepsPerFrame)
    {
        input.update();
        robot.update();
        field.update();
        b2World_Step(worldId, fixedDt, 4);
        accum -= fixedDt;
        steps++;
    }
}

Sim &Sim::getInstance()
{
    // Construct on first use to avoid Static Initialization Order Fiasco.
    static Sim sim;
    return sim;
}
} // namespace bromine
