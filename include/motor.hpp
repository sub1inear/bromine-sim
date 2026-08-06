#pragma once
#include "device.hpp"
#include <cstdint>

namespace cobalt
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

    uint64_t id;
    uint8_t port;

    Motor(const json &j);
    void update(float dt) override;
    void draw(Renderer &renderer, const DeviceContext &context) const override;
};
} // namespace cobalt
