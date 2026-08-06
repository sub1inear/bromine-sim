#include "field.hpp"
#include "cup.hpp"
#include "pin.hpp"
#include <numbers>

namespace cobalt
{
// Could use the Raylib constant but better to separate frontend and backend.
constexpr float radToDeg = std::numbers::pi_v<float> / 180.0f;

template <typename T, typename... A>
static std::vector<std::unique_ptr<T>> makeVector(std::unique_ptr<A>... args)
{
    std::vector<std::unique_ptr<T>> vec;
    vec.reserve(sizeof...(args));
    (vec.push_back(std::move(args)), ...);
    return vec;
}

Field::Field()
{
    objects = makeVector<Object>(
        std::make_unique<HorizontalPin>(-47.0f, 51.0f, 180.0f * radToDeg,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(-43.0f, 47.0f, 270.0f * radToDeg,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(-51.0f, 47.0f, 90.0f * radToDeg,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(-47.0f, 43.0f, 0.0f * radToDeg,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(-23.5f, 27.5f, 180.0f * radToDeg,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(-19.5f, 23.5f, 270.0f * radToDeg,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(-27.5f, 23.5f, 90.0f * radToDeg,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(-23.5f, 19.5f, 0.0f * radToDeg,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(23.5f, -19.5f, 180.0f * radToDeg,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(27.5f, -23.5f, 270.0f * radToDeg,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(19.5f, -23.5f, 90.0f * radToDeg,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(23.5f, -27.5f, 0.0f * radToDeg,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(47.0f, -43.0f, 180.0f * radToDeg,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(51.0f, -47.0f, 270.0f * radToDeg,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(43.0f, -47.0f, 90.0f * radToDeg,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(47.0f, -51.0f, 0.0f * radToDeg,
                                        Pin::Type::RedYellow),

        std::make_unique<Cup>(-47.0f, 47.0f, true),
        std::make_unique<Cup>(-23.5f, 23.5f, true),
        std::make_unique<Cup>(23.5f, -23.5f, true),
        std::make_unique<Cup>(47.0f, -47.0f, true));
}

void Field::update(float dt)
{
    for (const std::unique_ptr<Object> &object : objects)
    {
        object->update(dt);
    }
}
} // namespace cobalt
