#include "rotation.hpp"
#include "constants.hpp"
#include "renderer.hpp"
#include "robot.hpp"
#include "utils.hpp"
#include <numbers>

namespace bromine
{

Rotation::Rotation(const json &j, Robot &robot)
    : Device(j, robot), ratio(expectJsonKey<float>(j, "ratio")),
      reversed(expectJsonKey<bool>(j, "reversed")),
      id(expectJsonKey<uint32_t>(j, "id")), port(expectJsonPort(j)), sdk{}
{
    // Measured on https://www.vexrobotics.com/276-6050.html.
    rect.width = inputDistToPhysicsDist(0.7952756f);
    rect.height = inputDistToPhysicsDist(2.625984f);

    if (ratio <= 0)
    {
        error("rotation ratio must be greater than zero");
    }
}

void Rotation::preUpdate() {}

void Rotation::postUpdate()
{
    if (wheel != nullptr)
    {
        // Get the wheel's rect in world space.
        PhysicsRect rect = wheel->getWorldRect();

        // Find the rotated forward vector.
        PhysicsPoint forward = b2RotateVector(rect.pose.q, {0.0f, 1.0f});

        // Get the actual velocity of the wheel.
        PhysicsPoint wheelVelocity =
            b2Body_GetWorldPointVelocity(robot.rect.bodyId, rect.pose.p);
        // Isolate the forward component of the velocity and make it a
        // scalar.
        float speed = b2Dot(wheelVelocity, forward);

        PhysicsDist wheelRadius = rect.height / 2.0f;
        PhysicsDist wheelCircumference =
            2.0f * std::numbers::pi_v<float> * wheelRadius;
        float deltaPosition = speed * fixedDt / wheelCircumference * ratio;
        sdk.position += deltaPosition;
    }
}

void Rotation::draw(Renderer &renderer) const { renderer.drawRotation(*this); }

void Rotation::link()
{
    wheel = nullptr;
    for (auto &device : robot.devices)
    {
        auto testWheel = dynamic_cast<Wheel *>(device.get());
        if (testWheel != nullptr && testWheel->id == id)
        {
            if (wheel != nullptr)
            {
                error("rotation on port `{}` has more than one connected wheel",
                      port);
            }
            wheel = testWheel;
        }
    }
    if (wheel == nullptr)
    {
        warning("rotation on port `{}` has no connected wheels", port);
    }
}
} // namespace bromine
