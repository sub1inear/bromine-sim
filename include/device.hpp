#pragma once
#include "renderer.hpp"
#include "units.hpp"
#include "utils.hpp"
#include <box2d/box2d.h>
#include <cstdint>

namespace bromine
{

struct DeviceContext
{
    PhysicsPose pose;
};

class Device
{
public:
    PhysicsRect rect;

    Device(const json &j);
    PhysicsRect getWorldRect(const DeviceContext &context) const;
    virtual void update(float dt) = 0;
    virtual void
    draw(Renderer &renderer,
         const DeviceContext &context) const = 0; // Double-dispatch.
    virtual ~Device() = default;
};
} // namespace bromine
