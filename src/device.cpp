#include "device.hpp"
#include "robot.hpp"
#include <cmath>

namespace bromine
{
Device::Device(const json &j, Robot &robot) : robot(robot)
{
    InputRect inputRect = {expectJsonKey<float>(j, "x"),
                           expectJsonKey<float>(j, "y"),
                           expectJsonKey<float>(j, "angle"), 0.0f, 0.0f};
    rect = inputRectToPhysicsRect(inputRect);
}

PhysicsRect Device::getWorldRect() const
{
    // Transform the device to the robot coordinate system.
    return {b2MulTransforms(b2Body_GetTransform(robot.rect.bodyId), rect.pose),
            rect.width, rect.height};
}

} // namespace bromine
