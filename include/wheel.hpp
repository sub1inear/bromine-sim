#include "device.hpp"

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
    void update(float dt) override;
    void draw(Renderer &renderer, const DeviceContext &context) const override;
};
} // namespace bromine
