#include "wheel.hpp"
#include "robot.hpp"
#include "utils.hpp"

namespace bromine
{
Wheel::Wheel(const json &j, Robot &robot)
    : Device(j, robot), rpm(expectJsonKey<uint32_t>(j, "rpm")),
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

void Wheel::update() {}

void Wheel::draw(Renderer &renderer) const { renderer.drawWheel(*this); }

} // namespace bromine
