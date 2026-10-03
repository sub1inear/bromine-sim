#include "state.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include <algorithm>
#include <raylib.h>

namespace bromine
{
State::State()
    : data({Type::Practice, Phase::Disabled}), timer(0.0f), started(false)
{
}

void State::update()
{
    Type type = data.load().type;
    Phase phase = data.load().phase;

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
                    data = {type, Phase::Disabled};
                    break;
                case Phase::Disabled:
                    timer = 105.0f;
                    data = {type, Phase::Opcontrol};
                    break;
                case Phase::Opcontrol:
                    data = {type, Phase::Disabled};
                    break;
                default:
                    error("unexpected phase `{}`", static_cast<int>(phase));
                    break;
                }
            }
            else if (type != Type::Practice)
            {
                data = {type, Phase::Disabled};
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ONE))
        {
            data = {Type::Practice, Phase::Disabled};
            timer = 0.0f;
        }
        if (IsKeyPressed(KEY_TWO))
        {
            data = {Type::Match, Phase::Disabled};
            timer = 15.0f;
        }

        if (IsKeyPressed(KEY_THREE))
        {
            data = {Type::DriverSkills, Phase::Disabled};
            timer = 60.0f;
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            data = {Type::AutonomousSkills, Phase::Disabled};
            timer = 60.0f;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            started = true;
            switch (type)
            {
            case Type::Practice:
                data = {type, Phase::Opcontrol};
                break;
            case Type::Match:
                data = {type, Phase::Autonomous};
                break;
            case Type::AutonomousSkills:
                data = {type, Phase::Autonomous};
                break;
            case Type::DriverSkills:
                data = {type, Phase::Opcontrol};
                break;
            default:
                error("unknown state type `{}`", static_cast<int>(type));
                break;
            }
        }
    }
}
} // namespace bromine
