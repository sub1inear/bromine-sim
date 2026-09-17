#pragma once
#include "renderer.hpp"
#include "units.hpp"
#include "utils.hpp"
#include <box2d/box2d.h>
#include <cstdint>

namespace bromine
{
class Robot;

class Device
{
public:
    PhysicsRect rect;

    Device(const json &j);
    PhysicsRect getWorldRect(PhysicsPose pose) const;
    virtual void update(float dt, Robot &robot) = 0;
    virtual void draw(Renderer &renderer,
                      PhysicsPose parentPose) const = 0; // Double-dispatch.
    virtual ~Device() = default;
};
} // namespace bromine
