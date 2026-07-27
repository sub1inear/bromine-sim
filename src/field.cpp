#include "field.hpp"
#include "pin.hpp"
#include <numbers>

namespace cobalt
{
// Could use the Raylib constant but better to separate frontend and backend.
constexpr float radToDeg = std::numbers::pi_v<float> / 180.0f;

Field::Field()
{
    objects = {
        Pin{-47.0f, 51.0f, 180.0f * radToDeg, Pin::Kind::BlueYellow},
        Pin{-43.0f, 47.0f, 270.0f * radToDeg, Pin::Kind::BlueYellow},
        Pin{-51.0f, 47.0f, 90.0f * radToDeg, Pin::Kind::RedYellow},
        Pin{-47.0f, 43.0f, 0.0f * radToDeg, Pin::Kind::RedYellow},

        Pin{-23.5f, 27.5f, 180.0f * radToDeg, Pin::Kind::BlueYellow},
        Pin{-19.5f, 23.5f, 270.0f * radToDeg, Pin::Kind::BlueYellow},
        Pin{-27.5f, 23.5f, 90.0f * radToDeg, Pin::Kind::RedYellow},
        Pin{-23.5f, 19.5f, 0.0f * radToDeg, Pin::Kind::RedYellow},

        Pin{23.5f, -19.5f, 180.0f * radToDeg, Pin::Kind::BlueYellow},
        Pin{27.5f, -23.5f, 270.0f * radToDeg, Pin::Kind::BlueYellow},
        Pin{19.5f, -23.5f, 90.0f * radToDeg, Pin::Kind::RedYellow},
        Pin{23.5f, -27.5f, 0.0f * radToDeg, Pin::Kind::RedYellow},

        Pin{47.0f, -43.0f, 180.0f * radToDeg, Pin::Kind::BlueYellow},
        Pin{51.0f, -47.0f, 270.0f * radToDeg, Pin::Kind::BlueYellow},
        Pin{43.0f, -47.0f, 90.0f * radToDeg, Pin::Kind::RedYellow},
        Pin{47.0f, -51.0f, 0.0f * radToDeg, Pin::Kind::RedYellow},

        Cup{-47.0f, 47.0f, true},
        Cup{-23.5f, 23.5f, true},
        Cup{23.5f, -23.5f, true},
        Cup{47.0f, -47.0f, true},
    };
}
void Field::update(float dt)
{
    for (auto &object : objects)
    {
        std::visit([=](auto &concrete) { concrete.update(dt); }, object);
    }
}
} // namespace cobalt
