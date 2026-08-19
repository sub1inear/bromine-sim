#pragma once
#include "rect.hpp"
#include "renderer.hpp"
#include "utils.hpp"
#include <box2d/box2d.h>
#include <cstdint>

namespace cobalt
{

struct DeviceContext
{
    b2Transform transform;
};

class Device
{
public:
    Rect rect;

    Device(const json &j);
    Rect getWorldRect(const DeviceContext &context) const;
    virtual void update(float dt) = 0;
    virtual void
    draw(Renderer &renderer,
         const DeviceContext &context) const = 0; // Double-dispatch.
    virtual ~Device() = default;
};
} // namespace cobalt
