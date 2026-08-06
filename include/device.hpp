#pragma once
#include "renderer.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <cstdint>

namespace cobalt
{

struct DeviceContext
{
    RotatedRect rect;
};

class Device
{
public:
    RotatedRect rect;

    Device(const json &j)
        : rect(expectJsonKey<float>(j, "x"), expectJsonKey<float>(j, "y"), 0.0f,
               0.0f, expectJsonKey<float>(j, "theta"))
    {
    }
    RotatedRect getGlobalPosition(const DeviceContext &context) const;
    virtual void update(float dt) = 0;
    virtual void
    draw(Renderer &renderer,
         const DeviceContext &context) const = 0; // Double-dispatch.
    virtual ~Device() = default;
};
} // namespace cobalt
