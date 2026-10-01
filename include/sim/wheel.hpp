#pragma once

#include "device.hpp"
#include "units.hpp"

namespace bromine
{
class Wheel : public Device
{
public:
    enum class Subtype
    {
        Omni,
        Traction,
        Flex,
        // Mecanum wheels are unsupported.
    };
    Subtype subtype;
    uint32_t id;
    float size;
    float ratio;

    Wheel(const json &j, Robot &robot);
    void update() override;
    void draw(Renderer &renderer) const override;

private:
    float linearFriction;
    float lateralFriction;
};
} // namespace bromine
