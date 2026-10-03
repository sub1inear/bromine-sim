#pragma once

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

    Type type;
    Phase phase;
    float timer; // In seconds.
    bool started;

    State();
    void update();
};
} // namespace bromine
