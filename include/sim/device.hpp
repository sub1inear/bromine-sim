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
    Robot &robot;

    Device(const json &j, Robot &robot);
    PhysicsRect getWorldRect() const;
    virtual void preUpdate() = 0;
    virtual void postUpdate() = 0;
    virtual void draw(Renderer &renderer) const = 0; // Double-dispatch.
    virtual ~Device() = default;
};
} // namespace bromine
