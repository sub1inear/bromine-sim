#pragma once
#include "device.hpp"
#include "wheel.hpp"
#include <atomic>
#include <cstdint>
#include <vector>

namespace bromine
{

class Rotation : public Device
{
public:
    float ratio;
    bool reversed;

    uint32_t id;
    uint8_t port;

    struct Sdk
    {
        // In radians, unwrapped.
        std::atomic<float> position;
    };

    Sdk sdk;

    Rotation(const json &j, Robot &robot);
    void preUpdate() override;
    void postUpdate() override;
    void draw(Renderer &renderer) const override;

    void link();

private:
    Wheel *wheel;
};

} // namespace bromine
