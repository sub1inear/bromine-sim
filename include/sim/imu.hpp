#pragma once
#include "device.hpp"
#include <atomic>

namespace bromine
{
class Imu : public Device
{
public:
    uint8_t port;

    struct Sdk
    {
        // TODO: Convert to degrees here rather than in the SDK getter.
        // In radians, CW, not wrapped.
        std::atomic<float> heading;
        // In radians, CW, wrapped.
        std::atomic<float> rotation;
    };

    Sdk sdk;

    Imu(const json &j, Robot &robot);
    void update() override;
    void draw(Renderer &renderer) const override;

private:
    // In radians, CCW, wrapped.
    float previousHeading;
};
} // namespace bromine
