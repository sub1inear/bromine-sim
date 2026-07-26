#include "field.hpp"
#include <numbers>

namespace cobalt
{
// Could use the Raylib constant but better to separate frontend and backend.
constexpr float radToDeg = std::numbers::pi_v<float> / 180.0f;

Field::Field()
{
    pins = {
        {-47.0f, 51.0f, 180.0f * radToDeg, Pin::Kind::BlueYellow},
        {-43.0f, 47.0f, 270.0f * radToDeg, Pin::Kind::BlueYellow},
        {-51.0f, 47.0f, 90.0f * radToDeg, Pin::Kind::RedYellow},
        {-47.0f, 43.0f, 0.0f * radToDeg, Pin::Kind::RedYellow},

        {-23.5f, 27.5f, 180.0f * radToDeg, Pin::Kind::BlueYellow},
        {-19.5f, 23.5f, 270.0f * radToDeg, Pin::Kind::BlueYellow},
        {-27.5f, 23.5f, 90.0f * radToDeg, Pin::Kind::RedYellow},
        {-23.5f, 19.5f, 0.0f * radToDeg, Pin::Kind::RedYellow},

        {23.5f, -19.5f, 180.0f * radToDeg, Pin::Kind::BlueYellow},
        {27.5f, -23.5f, 270.0f * radToDeg, Pin::Kind::BlueYellow},
        {19.5f, -23.5f, 90.0f * radToDeg, Pin::Kind::RedYellow},
        {23.5f, -27.5f, 0.0f * radToDeg, Pin::Kind::RedYellow},

        {47.0f, -43.0f, 180.0f * radToDeg, Pin::Kind::BlueYellow},
        {51.0f, -47.0f, 270.0f * radToDeg, Pin::Kind::BlueYellow},
        {43.0f, -47.0f, 90.0f * radToDeg, Pin::Kind::RedYellow},
        {47.0f, -51.0f, 0.0f * radToDeg, Pin::Kind::RedYellow},
    };
}
void Field::update(float dt)
{
    for (Pin &pin : pins)
    {
        pin.update(dt);
    }
}
} // namespace cobalt
