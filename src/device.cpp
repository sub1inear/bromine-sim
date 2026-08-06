#include "device.hpp"

namespace cobalt
{
RotatedRect Device::getGlobalPosition(const DeviceContext &context) const
{
    float cosTheta = std::cos(context.rect.theta);
    float sinTheta = std::sin(context.rect.theta);

    Pose offsets = {rect.x * cosTheta - rect.y * sinTheta,
                    rect.x * sinTheta + rect.y * cosTheta, rect.theta};

    return {context.rect.x + offsets.x, context.rect.y - offsets.y,
            context.rect.theta + offsets.theta, rect.w, rect.h};
}

} // namespace cobalt
