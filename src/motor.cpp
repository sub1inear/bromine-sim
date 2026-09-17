#include "motor.hpp"
#include "robot.hpp"
#include "utils.hpp"
#include "wheel.hpp"

namespace bromine
{
Motor::Motor(const json &j)
    : Device(j), port(expectJsonKey<uint8_t>(j, "port")),
      id(expectJsonKey<uint32_t>(j, "id"))
{
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

void Motor::update(float dt, Robot &robot)
{
    for (auto &device : robot.devices)
    {
        if (Wheel *wheel = dynamic_cast<Wheel *>(device.get()))
        {
            if (wheel->id == id)
            {
                PhysicsRect rect =
                    wheel->getWorldRect(b2Body_GetTransform(robot.rect.bodyId));
                PhysicsPoint force = b2RotateVector(rect.pose.q, {0.0f, 0.01f});

                b2Body_ApplyForce(robot.rect.bodyId, force, rect.pose.p, true);
            }
        }
    }
}

void Motor::draw(Renderer &renderer, PhysicsPose parentPose) const
{
    renderer.drawMotor(*this, parentPose);
}

} // namespace bromine
