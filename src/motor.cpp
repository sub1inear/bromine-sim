#include "motor.hpp"
#include "utils.hpp"

namespace cobalt
{
Motor::Motor(nlohmann::json &json) : Device(json)
{
    std::string roleStr = expectJsonKey<std::string>(json, "motor", "role");
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

    std::string typeStr = expectJsonKey<std::string>(json, "motor", "type");
    if (typeStr == "11w")
    {
        type = Type::FullWatt;
    }
    else if (typeStr == "5.5w")
    {
        type = Type::HalfWatt;
    }
    else
    {
        error("unknown motor type `{}`, expected one of: `11w`, `5.5w`",
              typeStr);
    }
}

void Motor::update(float dt) {}
void Motor::draw(cobalt::Renderer &) const {}

} // namespace cobalt
