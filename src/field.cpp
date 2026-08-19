#include "field.hpp"
#include "constants.hpp"
#include "cup.hpp"
#include "pin.hpp"
#include <numbers>

namespace cobalt
{
// Could use the Raylib constant but better to separate frontend and backend.
constexpr float degToRad = std::numbers::pi_v<float> / 180.0f;

template <typename T, typename... A>
static std::vector<std::unique_ptr<T>> makeVector(std::unique_ptr<A>... args)
{
    std::vector<std::unique_ptr<T>> vec;
    vec.reserve(sizeof...(args));
    (vec.push_back(std::move(args)), ...);
    return vec;
}

static IdRect makeWall(b2WorldId worldId, float x, float y, float theta)
{
    IdRect rect;
    rect.width = fieldWallSize;
    rect.height = fieldSize;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = {inchesToMeters(x), inchesToMeters(y)};
    bodyDef.rotation = b2MakeRot(theta);
    rect.bodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon boxDef = b2MakeBox(rect.width / 2.0f, rect.height / 2.0f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    b2ShapeId shapeId = b2CreatePolygonShape(rect.bodyId, &shapeDef, &boxDef);

    return rect;
}

Field::Field(b2WorldId worldId)
{
    objects = makeVector<Object>(
        std::make_unique<HorizontalPin>(
            worldId, -47.0f, 51.0f, 180.0f * degToRad, Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(
            worldId, -43.0f, 47.0f, 270.0f * degToRad, Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -51.0f, 47.0f,
                                        90.0f * degToRad, Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, -47.0f, 43.0f, 0.0f * degToRad,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(
            worldId, -23.5f, 27.5f, 180.0f * degToRad, Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(
            worldId, -19.5f, 23.5f, 270.0f * degToRad, Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -27.5f, 23.5f,
                                        90.0f * degToRad, Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, -23.5f, 19.5f, 0.0f * degToRad,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(
            worldId, 23.5f, -19.5f, 180.0f * degToRad, Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(
            worldId, 27.5f, -23.5f, 270.0f * degToRad, Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 19.5f, -23.5f,
                                        90.0f * degToRad, Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, 23.5f, -27.5f, 0.0f * degToRad,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(
            worldId, 47.0f, -43.0f, 180.0f * degToRad, Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(
            worldId, 51.0f, -47.0f, 270.0f * degToRad, Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 43.0f, -47.0f,
                                        90.0f * degToRad, Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, 47.0f, -51.0f, 0.0f * degToRad,
                                        Pin::Type::RedYellow),

        std::make_unique<Cup>(worldId, -47.0f, 47.0f, true),
        std::make_unique<Cup>(worldId, -23.5f, 23.5f, true),
        std::make_unique<Cup>(worldId, 23.5f, -23.5f, true),
        std::make_unique<Cup>(worldId, 47.0f, -47.0f, true));
    // walls = {
    //     makeWall(worldId, 71.0f, 0.0f, 270.0f * degToRad),
    //     makeWall(worldId, 0.0f, 71.0f, 0.0f * degToRad),
    //     makeWall(worldId, -71.0f, 0.0f, 90.0f * degToRad),
    //     makeWall(worldId, 0.0f, -71.0f, 180.0f * degToRad),
    // };
}

void Field::update(float dt)
{
    for (const std::unique_ptr<Object> &object : objects)
    {
        object->update(dt);
    }
}
} // namespace cobalt
