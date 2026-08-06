#include "motor.hpp"
#include "utils.hpp"

namespace cobalt
{
Motor::Motor(const json &j)
    : Device(j), port(expectJsonKey<uint8_t>(j, "port")),
      id(expectJsonKey<uint32_t>(j, "id"))
{
    std::string_view roleStr = expectJsonKey<std::string_view>(j, "role");
    if (roleStr == "input")
    {
        role = Role::Input;
    }
    else if (roleStr == "output")
    {
        role = Role::Output;
    }
    else if (roleStr == "drive")
    {
        role = Role::Drive;
    }
    else
    {
        error("unknown motor role `{}`, expected one of: `input`, `output`, "
              "`drive`",
              roleStr);
    }

    std::string_view powerStr = expectJsonKey<std::string_view>(j, "power");
    if (powerStr == "full")
    {
        power = Power::Full;
        rect.w = 2.26f;
        rect.h = 2.82f;
    }
    else if (powerStr == "half")
    {
        power = Power::Half;
        rect.w = 2.25f;
        rect.h = 2.5f;
    }
    else
    {
        error("unknown motor power `{}`, expected one of: `full`, `half`",
              powerStr);
    }
}

void Motor::update(float dt) {}
void Motor::draw(Renderer &renderer, const DeviceContext &context) const
{
    renderer.drawMotor(*this, context);
}

} // namespace cobalt
