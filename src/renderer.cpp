#include "renderer.hpp"
#include "circle.hpp"
#include "constants.hpp"
#include "cup.hpp"
#include "motor.hpp"
#include "pin.hpp"
#include "rect.hpp"
#include "sim.hpp"
#include "utils.hpp"
#include "wheel.hpp"
#include <cassert>
#include <cmath>
#include <raymath.h>
#include <rlgl.h>
#include <utility>

namespace cobalt
{

constexpr Color fieldRed = {209, 46, 81, 255};
constexpr Color fieldBlue = {87, 156, 189, 255};
constexpr Color fieldYellow = {199, 191, 120, 255};

template <typename T>
static T remapDist(T dist, T fromMin, T fromMax, T toMin, T toMax)
{
    T fromRange = fromMax - fromMin;
    T toRange = toMax - toMin;
    return ((dist * toRange) / fromRange) + toMin;
}

template <typename T>
static T remapValue(T value, T fromMin, T fromMax, T toMin, T toMax)
{
    value = std::clamp(value, fromMin, fromMax);
    value -= fromMin;
    return remapDist(value, fromMin, fromMax, toMin, toMax);
}

static float distToScreen(float dist)
{
    assert(GetScreenWidth() == GetScreenHeight());
    float screenWidth = static_cast<float>(GetScreenWidth());

    return remapDist(dist, fieldWallMin, fieldWallMax, 0.0f, screenWidth);
}

static Vector2 b2Vec2ToScreen(b2Vec2 point)
{
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    return {
        remapValue(point.x, fieldWallMin, fieldWallMax, 0.0f, screenWidth),
        remapValue(-point.y, fieldWallMin, fieldWallMax, 0.0f, screenHeight),
    };
}

static float b2RotToScreen(b2Rot rotation)
{
    return ccwToCw(b2Rot_GetAngle(rotation) * RAD2DEG);
}

static std::pair<Vector2, float> b2TransformToScreen(b2Transform transform)
{
    return std::make_pair(b2Vec2ToScreen(transform.p),
                          b2RotToScreen(transform.q));
}
static std::pair<Vector2, float> circleToScreen(Circle circle)
{
    return std::make_pair(b2Vec2ToScreen(circle.transform.p),
                          distToScreen(circle.radius));
}

static std::pair<Vector2, float> idCircleToScreen(IdCircle circle)
{
    b2Transform transform = b2Body_GetTransform(circle.bodyId);
    return circleToScreen({transform, circle.radius});
}

static std::pair<Rectangle, float> rectToScreen(Rect rect)
{
    auto [vec, angle] = b2TransformToScreen(rect.transform);
    return std::make_pair(Rectangle{vec.x, vec.y, distToScreen(rect.width),
                                    distToScreen(rect.height)},
                          angle);
}

static std::pair<Rectangle, float> idRectToScreen(IdRect rect)
{
    b2Transform transform = b2Body_GetTransform(rect.bodyId);
    return rectToScreen({transform, rect.width, rect.height});
}

static void DrawRectangleLinesPro(Rectangle rect, Vector2 origin,
                                  float rotation, float thick, Color color)
{
    rlPushMatrix();
    {
        rlTranslatef(rect.x, rect.y, 0.0f);
        rlRotatef(rotation, 0.0f, 0.0f, 1.0f);
        rlTranslatef(-origin.x, -origin.y, 0.0f);
        DrawRectangleLinesEx({0.0f, 0.0f, rect.width, rect.height}, thick,
                             color);
    }
    rlPopMatrix();
}

Renderer::Renderer()
{
    fieldTexture = LoadTexture("assets/field.png");
    pinsTexture = LoadTexture("assets/pins.png");
    GenTextureMipmaps(&pinsTexture);
    SetTextureFilter(pinsTexture, TEXTURE_FILTER_TRILINEAR);
}

Renderer::~Renderer() { UnloadTexture(fieldTexture); }

void Renderer::draw(const Sim &sim)
{
    BeginDrawing();
    {
        drawField(sim.field);
        drawRobot(sim.robot);
    }
    EndDrawing();
}

void Renderer::drawField(const Field &field)
{
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    float fieldTextureWidth = static_cast<float>(fieldTexture.width);
    float fieldTextureHeight = static_cast<float>(fieldTexture.height);

    Rectangle source = {0.0f, 0.0f, fieldTextureWidth, fieldTextureHeight};
    Rectangle dest = {0.0f, 0.0f, screenWidth, screenHeight};
    Vector2 origin = {0.0f, 0.0f};
    DrawTexturePro(fieldTexture, source, dest, origin, 0.0f, WHITE);

    // for (const auto &object : field.objects)
    // {
    //     drawObject(*object);
    // }
}

void Renderer::drawRobot(const Robot &robot)
{
    auto [rect, angle] = idRectToScreen(robot.rect);

    Vector2 origin = {rect.width / 2.0f, rect.height / 2.0f};
    DrawRectanglePro(rect, origin, angle, WHITE);

    for (const auto &device : robot.devices)
    {
        DeviceContext deviceContext = {b2Body_GetTransform(robot.rect.bodyId)};
        drawDevice(*device, deviceContext);
    }
}

void Renderer::drawObject(const Object &object)
{
    object.draw(*this);
    if (object.next)
    {
        drawObject(*object.next);
    }
}

void Renderer::drawHorizontalPin(const HorizontalPin &pin)
{
    auto [dest, theta] = idRectToScreen(pin.rect);
    Rectangle source = {static_cast<float>(pin.type) * 467.0f, 0.0f, 467.0f,
                        static_cast<float>(pinsTexture.height)};

    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
    DrawTexturePro(pinsTexture, source, dest, origin, theta, WHITE);
}

void Renderer::drawVerticalPin(const VerticalPin &pin)
{
    auto [vec, radius] = idCircleToScreen(pin.circle);
    Color color = WHITE;
    switch (pin.type)
    {
    case Pin::Type::YellowYellow:
        color = fieldYellow;
        break;
    case Pin::Type::RedYellow:
        color = pin.isUp ? fieldRed : fieldYellow;
        break;
    case Pin::Type::BlueYellow:
        color = pin.isUp ? fieldBlue : fieldYellow;
        break;
    case Pin::Type::RedBlue:
        color = pin.isUp ? fieldRed : fieldBlue;
        break;
    }
    DrawCircleV(vec, radius, color);
}

void Renderer::drawCup(const Cup &cup)
{
    auto [vec, radius] = idCircleToScreen(cup.circle);
    Color color = cup.up ? LIGHTGRAY : DARKGRAY;
    DrawCircleV(vec, radius, color);
}

void Renderer::drawMotor(const Motor &motor, const DeviceContext &context)
{
    Rect rect = motor.getWorldRect(context);
    auto [dest, theta] = rectToScreen(rect);

    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};

    Color color = WHITE;
    switch (motor.role)
    {
    case Motor::Role::Input:
        color = ORANGE;
        break;
    case Motor::Role::Output:
        color = BLUE;
        break;
    case Motor::Role::Drive:
        color = GREEN;
        break;
    }

    DrawRectangleLinesPro(dest, origin, theta, 1.5f, color);
}

void Renderer::drawWheel(const Wheel &wheel, const DeviceContext &context)
{
    Rect rect = wheel.getWorldRect(context);
    auto [dest, theta] = rectToScreen(rect);

    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};

    DrawRectangleLinesPro(dest, origin, theta, 1.5f, PURPLE);
}

void Renderer::drawDevice(const Device &device, const DeviceContext &context)
{
    device.draw(*this, context);
}

} // namespace cobalt
