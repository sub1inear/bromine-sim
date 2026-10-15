#include "field.hpp"
#include "categories.hpp"
#include "constants.hpp"
#include "cup.hpp"
#include "pin.hpp"
#include <numbers>

namespace bromine
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
    shapeDef.filter.categoryBits = std::to_underlying(Categories::Wall);

    return inputRectToPhysicsIdRect(rect, worldId, bodyDef, shapeDef);
}

static PhysicsIdPolygon makeGoal(b2WorldId worldId, InputDist x, InputDist y)
{
    InputPolygon polygon = {
        x,
        y,
        0.0f,
        0.0f,
        {{1.75f, 2.75f},
         {2.75f, 1.75f},
         {2.75f, -1.75f},
         {1.75f, -2.75f},
         {-1.75f, -2.75f},
         {-2.75f, -1.75f},
         {-2.75f, 1.75f},
         {-1.75f, 2.75f}},
    };

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.filter.categoryBits = std::to_underlying(Categories::Goal);

    return inputPolygonToPhysicsIdPolygon(polygon, worldId, bodyDef, shapeDef);
}

Field::Field(b2WorldId worldId)
{
    objects = makeVector<Object>(
        std::make_unique<HorizontalPin>(worldId, -47.0f, 52.0f, 180.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -42.0f, 47.0f, 270.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -52.0f, 47.0f, 90.0f,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, -47.0f, 42.0f, 0.0f,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(worldId, -23.5f, 28.5f, 180.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -18.5f, 23.5f, 270.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, -28.5f, 23.5f, 90.0f,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, -23.5f, 18.5f, 0.0f,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(worldId, 23.5f, -18.5f, 180.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 28.5f, -23.5f, 270.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 18.5f, -23.5f, 90.0f,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, 23.5f, -28.5f, 0.0f,
                                        Pin::Type::RedYellow),

        std::make_unique<HorizontalPin>(worldId, 47.0f, -42.0f, 180.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 52.0f, -47.0f, 270.0f,
                                        Pin::Type::BlueYellow),
        std::make_unique<HorizontalPin>(worldId, 42.0f, -47.0f, 90.0f,
                                        Pin::Type::RedYellow),
        std::make_unique<HorizontalPin>(worldId, 47.0f, -52.0f, 0.0f,
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
    goals = {
        makeGoal(worldId, 0.00f, 0.00f),     makeGoal(worldId, 23.25f, 46.50f),
        makeGoal(worldId, -23.25f, 46.50f),  makeGoal(worldId, 23.25f, -46.50f),
        makeGoal(worldId, -23.25f, -46.50f), makeGoal(worldId, 46.50f, 23.25f),
        makeGoal(worldId, -46.50f, 23.25f),  makeGoal(worldId, 46.50f, -23.25f),
        makeGoal(worldId, -46.50f, -23.25f)};
}

void Field::preUpdate()
{
    for (const std::unique_ptr<Object> &object : objects)
    {
        object->preUpdate();
    }
}

void Field::postUpdate()
{
    for (const std::unique_ptr<Object> &object : objects)
    {
        object->postUpdate();
    }
}
} // namespace bromine
