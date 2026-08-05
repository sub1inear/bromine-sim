#pragma once
#include "device.hpp"

namespace cobalt
{
class Motor : public Device
{
  public:
    enum class Role
    {
        Input,
        Output,
        Drive,
    };
    enum class Type
    {
        FullWatt,
        HalfWatt,
    };
    Role role;
    Type type;
    Motor(nlohmann::json &json);
    void update(float dt);
    void draw(Renderer &renderer) const;
};
} // namespace cobalt
