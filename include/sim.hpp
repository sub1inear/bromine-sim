#pragma once
#include "field.hpp"
#include "robot.hpp"

namespace cobalt
{
class Sim
{
public:
    Field field;
    Robot robot;

    void update(float dt);
};
} // namespace cobalt
