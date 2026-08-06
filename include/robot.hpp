#pragma once
#include "device.hpp"
#include "types.hpp"
#include <memory>
#include <vector>

namespace cobalt
{
class Robot
{
public:
    RotatedRect rect;
    Pose velocity;
    float mass; // In lbs.

    std::vector<std::unique_ptr<Device>> devices;

    Robot();
    void update(float dt);
};

} // namespace cobalt
