#include "device.hpp"
#include "utils.hpp"
#include <cmath>

namespace cobalt
{
Device::Device(const json &j)
{
    float theta = expectJsonKey<float>(j, "theta");
    rect = {{{inchesToMeters(expectJsonKey<float>(j, "x")),
              inchesToMeters(expectJsonKey<float>(j, "y"))},
             b2MakeRot(cwToCcw(theta))},
            0.0f,
            0.0f};
}
Rect Device::getWorldRect(const DeviceContext &context) const
{
    // Transform the device to the robot coordinate system.
    return {b2MulTransforms(context.transform, rect.transform), rect.width,
            rect.height};
}

} // namespace cobalt
