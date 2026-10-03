#pragma once
#include <atomic>

namespace bromine
{
class State
{
public:
    enum class Phase
    {
        Disabled,
        Autonomous,
        Opcontrol,
    };

    enum class Type
    {
        Practice,
        Match,
        AutonomousSkills,
        DriverSkills,
    };

    struct Data
    {
        Type type;
        Phase phase;
    };

    std::atomic<Data> data;

    float timer; // In seconds.
    bool started;

    State();
    void update();
};
} // namespace bromine
