#pragma once
#include "device.hpp"
#include "units.hpp"
#include <box2d/box2d.h>
#include <memory>
#include <vector>

namespace bromine
{
class Robot
{
public:
    PhysicsIdRect rect;
    std::vector<std::unique_ptr<Device>> devices;
    size_t wheelCount;

    Robot(b2WorldId worldId);
    void preUpdate();
    void postUpdate();
};

} // namespace bromine
