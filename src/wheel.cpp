#include "wheel.hpp"
#include "robot.hpp"
#include "utils.hpp"

namespace bromine
{
Wheel::Wheel(const json &j)
    : Device(j), rpm(expectJsonKey<uint32_t>(j, "rpm")),
      id(expectJsonKey<uint32_t>(j, "id"))
{
    std::string_view subtypeStr = expectJsonKey<std::string_view>(j, "subtype");
    if (subtypeStr == "omni")
    {
        subtype = Subtype::Omni;
    }
    else if (subtypeStr == "traction")
    {
        subtype = Subtype::Traction;
    }
    else if (subtypeStr == "flex")
    {
        subtype = Subtype::Flex;
    }
    rect.width = inchesToMeters(expectJsonKey<float>(j, "width"));
    rect.height = inchesToMeters(expectJsonKey<float>(j, "diameter"));
}

void Wheel::update(float dt, Robot &robot) {}

void Wheel::draw(Renderer &renderer, PhysicsPose parentPose) const
{
    renderer.drawWheel(*this, parentPose);
}

} // namespace bromine
