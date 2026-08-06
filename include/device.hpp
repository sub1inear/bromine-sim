#pragma once
#include "renderer.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <cstdint>
#include <utility>

namespace cobalt
{
class DeviceContext
{
public:
    Rect rect;
    float theta;
};

class Device
{
public:
    Rect rect;
    float theta;

    Device(const json &j)
        : rect(expectJsonKey<float>(j, "x"), expectJsonKey<float>(j, "y")),
          theta(expectJsonKey<float>(j, "theta"))
    {
    }
    std::pair<Rect, float>
    getGlobalPosition(const DeviceContext &context) const;
    virtual void update(float dt) = 0;
    virtual void
    draw(Renderer &renderer,
         const DeviceContext &context) const = 0; // Double-dispatch.
    virtual ~Device() = default;
};
} // namespace cobalt
