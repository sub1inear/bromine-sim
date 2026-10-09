#pragma once
#include "device.hpp"
#include "v5_api.h"
#include "wheel.hpp"
#include <atomic>
#include <cstdint>
#include <vector>

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

    V5MotorGearset gearset;
    Role role;
    Power power;

    uint32_t id;
    uint8_t port;

    bool reversed;

    struct Sdk
    {
        // TODO: Check default gearset and encoder units.
        std::atomic<int32_t> voltage;
        std::atomic<V5MotorGearset> gearset;
        std::atomic<float>
            position; // In rotations of the output shaft regardless of gearing.
        std::atomic<V5MotorEncoderUnits> encoderUnits;
    };

    Sdk sdk;

    Motor(const json &j, Robot &robot);
    void update() override;
    void draw(Renderer &renderer) const override;

    void link();

private:
    std::vector<Wheel *> wheels;
};
} // namespace bromine
