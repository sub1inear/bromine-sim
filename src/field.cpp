#include "field.hpp"
#include "constants.hpp"
#include "cup.hpp"
#include "pin.hpp"
#include <numbers>

namespace cobalt
{
template <typename T, typename... A>
static std::vector<std::unique_ptr<T>> makeVector(std::unique_ptr<A>... args)
{
    std::vector<std::unique_ptr<T>> vec;
    vec.reserve(sizeof...(args));
    (vec.push_back(std::move(args)), ...);
    return vec;
}

static PhysicsIdRect makeWall(b2WorldId worldId, InputDist x, InputDist y,
                              InputAngle angle)
{
    InputRect rect = {x, y, angle, inputWallSize, inputFieldWallSize};

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    return inputRectToPhysicsIdRect(rect, worldId, bodyDef, shapeDef);
}

Field::Field(b2WorldId worldId)
{
    objects = makeVector<Object>(
        std::make_unique<HorizontalPin>(worldId, -47.0f, 51.0f, 180.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -43.0f, 47.0f, 270.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -51.0f, 47.0f, 90.0f,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, -47.0f, 43.0f, 0.0f,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(worldId, -23.5f, 27.5f, 180.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -19.5f, 23.5f, 270.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -27.5f, 23.5f, 90.0f,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, -23.5f, 19.5f, 0.0f,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(worldId, 23.5f, -19.5f, 180.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 27.5f, -23.5f, 270.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 19.5f, -23.5f, 90.0f,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, 23.5f, -27.5f, 0.0f,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(worldId, 47.0f, -43.0f, 180.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 51.0f, -47.0f, 270.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 43.0f, -47.0f, 90.0f,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, 47.0f, -51.0f, 0.0f,
                                        Pin::Type::RedYellow),

        std::make_unique<Cup>(worldId, -47.0f, 47.0f, 0.0f, true),
        std::make_unique<Cup>(worldId, -23.5f, 23.5f, 0.0f, true),
        std::make_unique<Cup>(worldId, 23.5f, -23.5f, 0.0f, true),
        std::make_unique<Cup>(worldId, 47.0f, -47.0f, 0.0f, true));
    walls = {
        makeWall(worldId, 71.0f, 0.0f, 0.0f),
        makeWall(worldId, -71.0f, 0.0f, 0.0f),
        makeWall(worldId, 0.0f, -71.0f, 90.0f),
        makeWall(worldId, 0.0f, 71.0f, 90.0f),
    };
}

void Field::update(float dt)
{
    for (const std::unique_ptr<Object> &object : objects)
    {
        object->update(dt);
    }
}
} // namespace cobalt
