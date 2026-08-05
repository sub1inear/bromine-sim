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
    Motor(const nlohmann::json &json);
    void update(float dt) override;
    void draw(Renderer &renderer, const DeviceContext &context) const override;
};
} // namespace cobalt
