#include "device.hpp"
#include <cmath>

namespace cobalt
{
Device::Device(const json &j)
{
    InputRect inputRect = {expectJsonKey<float>(j, "x"),
                           expectJsonKey<float>(j, "y"),
                           expectJsonKey<float>(j, "theta"), 0.0f, 0.0f};
    rect = inputRectToPhysicsRect(inputRect);
}

PhysicsRect Device::getWorldRect(const DeviceContext &context) const
{
    // Transform the device to the robot coordinate system.
    return {b2MulTransforms(context.pose, rect.pose), rect.width, rect.height};
}

} // namespace cobalt
