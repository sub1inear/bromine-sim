#pragma once
#include "device.hpp"
#include "rect.hpp"
#include <box2d/box2d.h>
#include <memory>
#include <vector>

namespace cobalt
{
class Robot
{
public:
    IdRect rect;
    std::vector<std::unique_ptr<Device>> devices;

    Robot(b2WorldId worldId);
    void update(float dt);
};

} // namespace cobalt
