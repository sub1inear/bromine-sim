#include "state.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include <algorithm>
#include <raylib.h>

namespace bromine
{
State::State()
    : type(Type::Practice), phase(Phase::Opcontrol), timer(0.0f), started(false)
{
}

void State::update()
{
    if (started)
    {
        if (timer > 0.0f)
        {
            timer -= fixedDt;
            timer = std::max(timer, 0.0f);
        }
        else
        {
            timer = 0.0f;
            if (type == Type::Match)
            {
                switch (phase)
                {
                case Phase::Autonomous:
                    timer = 5.0f;
                    phase = Phase::Disabled;
                    break;
                case Phase::Disabled:
                    timer = 105.0f;
                    phase = Phase::Opcontrol;
                    break;
                default:
                    break;
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ONE))
        {
            type = Type::Practice;
            timer = 0.0f;
            phase = Phase::Opcontrol;
        }
        if (IsKeyPressed(KEY_TWO))
        {
            type = Type::Match;
            timer = 15.0f;
            phase = Phase::Autonomous;
        }

        if (IsKeyPressed(KEY_THREE))
        {
            type = Type::DriverSkills;
            timer = 60.0f;
            phase = Phase::Opcontrol;
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            type = Type::AutonomousSkills;
            timer = 60.0f;
            phase = Phase::Autonomous;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            started = true;
        }
    }
}
} // namespace bromine
