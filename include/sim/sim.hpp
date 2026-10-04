#pragma once
#include "field.hpp"
#include "input.hpp"
#include "robot.hpp"
#include "screen.hpp"
#include "state.hpp"
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
    State state;
    Screen screen;

    Sim();
    ~Sim();
    void update();
    static Sim &getInstance();
};

} // namespace bromine
