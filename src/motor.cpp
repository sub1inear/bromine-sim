#include "motor.hpp"
#include "utils.hpp"

namespace cobalt
{
Motor::Motor(const nlohmann::json &json) : Device(json)
{

    std::string roleStr = expectJsonKey<std::string>(json, "role");
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

    std::string typeStr = expectJsonKey<std::string>(json, "type");
    if (typeStr == "11w")
    {
        type = Type::FullWatt;
        rect.w = 2.26f;
        rect.h = 2.82f;
    }
    else if (typeStr == "5.5w")
    {
        type = Type::HalfWatt;
        rect.w = 2.25f;
        rect.h = 2.5f;
    }
    else
    {
        error("unknown motor type `{}`, expected one of: `11w`, `5.5w`",
              typeStr);
    }
}

void Motor::update(float dt) {}
void Motor::draw(Renderer &renderer, const DeviceContext &context) const
{
    renderer.drawMotor(*this, context);
}

} // namespace cobalt
