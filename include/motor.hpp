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

    Motor(const json &j, Robot &robot);
    void update(float dt) override;
    void draw(Renderer &renderer) const override;
};
} // namespace bromine
