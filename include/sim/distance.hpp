#pragma once
#include "device.hpp"
#include <atomic>

namespace bromine
{
class Robot;

class Distance : public Device
{
public:
    uint8_t port;

    PhysicsPoint lineStart;
    PhysicsPoint lineEnd;

    struct Sdk
    {
        std::atomic<int32_t> distance; // In mm.
    };

    Sdk sdk;

    Distance(const json &j, Robot &robot);
    void update() override;
    void draw(Renderer &renderer) const override;
};
} // namespace bromine
