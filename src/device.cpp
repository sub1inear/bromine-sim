#include "device.hpp"

namespace cobalt
{
std::pair<Rect, float>
Device::getGlobalPosition(const DeviceContext &context) const
{
    float c = std::cos(context.theta);
    float s = std::sin(context.theta);

    Pose offsets = {rect.x * c - rect.y * s, rect.x * s + rect.y * c, theta};

    return std::make_pair<Rect, float>({context.rect.x + offsets.x,
                                        context.rect.y - offsets.y, rect.w,
                                        rect.h},
                                       context.theta + offsets.theta);
}

} // namespace cobalt
