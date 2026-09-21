#include "motor.hpp"
#include "robot.hpp"
#include "utils.hpp"
#include "wheel.hpp"

namespace bromine
{
Motor::Motor(const json &j, Robot &robot)
    : Device(j, robot), id(expectJsonKey<uint32_t>(j, "id"))
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

void Motor::update(float dt) {}

void Motor::setVoltage(int32_t voltage)
{
    this->voltage = voltage;
    for (auto &device : robot.devices)
    {
        if (Wheel *wheel = dynamic_cast<Wheel *>(device.get()))
        {
            if (wheel->id == id)
            {
                PhysicsRect rect = wheel->getWorldRect();
                PhysicsPoint force = b2RotateVector(rect.pose.q, {0.0f, 1.0f});
                b2Body_ApplyForce(robot.rect.bodyId, force, rect.pose.p, true);
            }
        }
    }
}

int32_t Motor::getVoltage() { return voltage; }

void Motor::draw(Renderer &renderer) const { renderer.drawMotor(*this); }

} // namespace bromine
