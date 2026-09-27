#include "motor.hpp"
#include "robot.hpp"
#include "utils.hpp"
#include "wheel.hpp"

namespace bromine
{
Motor::Motor(const json &j, Robot &robot)
    : Device(j, robot), id(expectJsonKey<uint32_t>(j, "id")), voltage(0)
{
    port = expectJsonKey<uint8_t>(j, "port");
    if (port < 1 || port > 21)
    {
        error("motor port must be in range [1, 21], got {}", port);
    }

    std::string_view roleStr = expectJsonKey<std::string_view>(j, "role");
    if (roleStr == "input")
    {
        role = Role::Input;
    }
    else if (roleStr == "output")
    {
        role = Role::Output;
    }
    else if (roleStr == "drive")
    {
        role = Role::Drive;
    }
    else
    {
        error("unknown motor role `{}`, expected one of: `input`, `output`, "
              "`drive`",
              roleStr);
    }

    std::string_view powerStr = expectJsonKey<std::string_view>(j, "power");
    if (powerStr == "full")
    {
        power = Power::Full;
        rect.width = inputDistToPhysicsDist(2.26f);
        rect.height = inputDistToPhysicsDist(2.82f);
    }
    else if (powerStr == "half")
    {
        power = Power::Half;
        rect.width = inputDistToPhysicsDist(2.25f);
        rect.height = inputDistToPhysicsDist(2.5f);
    }
    else
    {
        error("unknown motor power `{}`, expected one of: `full`, `half`",
              powerStr);
    }
}

void Motor::update(float dt)
{
    if (voltage != 0)
    {
        for (Wheel *wheel : wheels)
        {
            PhysicsRect rect = wheel->getWorldRect();
            // TODO: Simulate a real Smart Motor, stubbed for now.
            PhysicsPoint force =
                b2RotateVector(rect.pose.q, {0.0f, voltage * 0.0002f});
            b2Body_ApplyForce(robot.rect.bodyId, force, rect.pose.p, true);
        }
    }
}

void Motor::draw(Renderer &renderer) const { renderer.drawMotor(*this); }

void Motor::link()
{
    wheels.clear();
    for (auto &device : robot.devices)
    {
        auto wheel = dynamic_cast<Wheel *>(device.get());
        if (wheel != nullptr && wheel->id == id)
        {
            wheels.push_back(wheel);
        }
    }
}

} // namespace bromine
