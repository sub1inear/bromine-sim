#pragma once
#include "renderer.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <cstdint>

namespace cobalt
{
class Device
{
public:
    uint8_t port;
    Pose pose;

    Device(nlohmann::json &json)
        : port(expectJsonKey<uint8_t>(json, "", "port")),
          pose(expectJsonKey<float>(json, "", "x"),
               expectJsonKey<float>(json, "", "y"),
               expectJsonKey<float>(json, "", "theta"))
    {
    }
    virtual void update(float dt) = 0;
    virtual void draw(Renderer &renderer) const = 0; // Double-dispatch.
    virtual ~Device() = default;
};
} // namespace cobalt
