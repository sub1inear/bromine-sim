#pragma once
#include "field.hpp"
#include "input.hpp"
#include "robot.hpp"
#include <box2d/box2d.h>

namespace bromine
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
    Input input;

    Sim();
    ~Sim();
    void update(float dt);
    static Sim &getInstance();
};

} // namespace bromine
