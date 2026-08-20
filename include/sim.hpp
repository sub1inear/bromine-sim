#pragma once
#include "field.hpp"
#include "robot.hpp"
#include <box2d/box2d.h>

namespace cobalt
{
class Sim
{
public:
    // worldId must be declared first because C++ initializers run in
    // declaration order regardless of positioning in constructor.
    b2WorldId worldId;

private:
    // Accumulated delta time.
    float accum;

public:
    Field field;
    Robot robot;

    Sim();
    ~Sim();
    void update(float dt);
};
} // namespace cobalt
