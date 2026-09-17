#pragma once
#include "device.hpp"
#include <cstdint>

namespace bromine
{
class Motor : public Device
{
public:
    enum class Role
    {
        Input,
        Output,
        Drive,
    };
    enum class Power
    {
        Full,
        Half,
    };
    Role role;
    Power power;

    uint32_t id;
    uint8_t port;

    Motor(const json &j);
    void update(float dt, Robot &robot) override;
    void draw(Renderer &renderer, PhysicsPose parentPose) const override;
};
} // namespace bromine
