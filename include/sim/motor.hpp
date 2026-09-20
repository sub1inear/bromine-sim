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

    int32_t voltage;

    Motor(const json &j, Robot &robot);
    void update(float dt) override;
    void setVoltage(int32_t voltage);
    int32_t getVoltage();
    void draw(Renderer &renderer) const override;
};
} // namespace bromine
