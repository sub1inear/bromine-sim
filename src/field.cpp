#include "field.hpp"
#include "cup.hpp"
#include "pin.hpp"
#include <numbers>

namespace cobalt
{
// Could use the Raylib constant but better to separate frontend and backend.
constexpr float radToDeg = std::numbers::pi_v<float> / 180.0f;

Field::Field()
{
    objects.push_back(std::make_unique<Pin>(-47.0f, 51.0f, 180.0f * radToDeg,
                                            Pin::Kind::BlueYellow));
    objects.push_back(std::make_unique<Pin>(-43.0f, 47.0f, 270.0f * radToDeg,
                                            Pin::Kind::BlueYellow));
    objects.push_back(std::make_unique<Pin>(-51.0f, 47.0f, 90.0f * radToDeg,
                                            Pin::Kind::RedYellow));
    objects.push_back(std::make_unique<Pin>(-47.0f, 43.0f, 0.0f * radToDeg,
                                            Pin::Kind::RedYellow));

    objects.push_back(std::make_unique<Pin>(-23.5f, 27.5f, 180.0f * radToDeg,
                                            Pin::Kind::BlueYellow));
    objects.push_back(std::make_unique<Pin>(-19.5f, 23.5f, 270.0f * radToDeg,
                                            Pin::Kind::BlueYellow));
    objects.push_back(std::make_unique<Pin>(-27.5f, 23.5f, 90.0f * radToDeg,
                                            Pin::Kind::RedYellow));
    objects.push_back(std::make_unique<Pin>(-23.5f, 19.5f, 0.0f * radToDeg,
                                            Pin::Kind::RedYellow));

    objects.push_back(std::make_unique<Pin>(23.5f, -19.5f, 180.0f * radToDeg,
                                            Pin::Kind::BlueYellow));
    objects.push_back(std::make_unique<Pin>(27.5f, -23.5f, 270.0f * radToDeg,
                                            Pin::Kind::BlueYellow));
    objects.push_back(std::make_unique<Pin>(19.5f, -23.5f, 90.0f * radToDeg,
                                            Pin::Kind::RedYellow));
    objects.push_back(std::make_unique<Pin>(23.5f, -27.5f, 0.0f * radToDeg,
                                            Pin::Kind::RedYellow));

    objects.push_back(std::make_unique<Pin>(47.0f, -43.0f, 180.0f * radToDeg,
                                            Pin::Kind::BlueYellow));
    objects.push_back(std::make_unique<Pin>(51.0f, -47.0f, 270.0f * radToDeg,
                                            Pin::Kind::BlueYellow));
    objects.push_back(std::make_unique<Pin>(43.0f, -47.0f, 90.0f * radToDeg,
                                            Pin::Kind::RedYellow));
    objects.push_back(std::make_unique<Pin>(47.0f, -51.0f, 0.0f * radToDeg,
                                            Pin::Kind::RedYellow));

    objects.push_back(std::make_unique<Cup>(-47.0f, 47.0f, true));
    objects.push_back(std::make_unique<Cup>(-23.5f, 23.5f, true));
    objects.push_back(std::make_unique<Cup>(23.5f, -23.5f, true));
    objects.push_back(std::make_unique<Cup>(47.0f, -47.0f, true));
}

void Field::update(float dt)
{
    for (const std::unique_ptr<Object> &object : objects)
    {
        object->update(dt);
    }
}
} // namespace cobalt
