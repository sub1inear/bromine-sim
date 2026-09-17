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
    uint32_t rpm;
    uint32_t id;
    float size;

    Wheel(const json &j);
    void update(float dt, Robot &robot) override;
    void draw(Renderer &renderer, PhysicsPose parentPose) const override;
};
} // namespace bromine
