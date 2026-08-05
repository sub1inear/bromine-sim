#include "renderer.hpp"
#include "constants.hpp"
#include "cup.hpp"
#include "motor.hpp"
#include "pin.hpp"
#include "sim.hpp"
#include "types.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <rlgl.h>
#include <variant>

namespace cobalt
{

constexpr Color fieldRed = {209, 46, 81, 255};
constexpr Color fieldBlue = {87, 156, 189, 255};
constexpr Color fieldYellow = {199, 191, 120, 255};

class DeviceContext
{
public:
    Rectangle rect;
    float theta; // Radians.
};

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

static float cobaltDistToDist(Dist dist)
{
    assert(GetScreenWidth() == GetScreenHeight());
    float screenWidth = static_cast<float>(GetScreenWidth());

    return remapDist(dist, fieldWallMin, fieldWallMax, 0.0f, screenWidth);
}

static Vector2 cobaltPointToVector2(Point point)
{
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    return {
        remapValue(point.x, fieldWallMin, fieldWallMax, 0.0f, screenWidth),
        remapValue(-point.y, fieldWallMin, fieldWallMax, 0.0f, screenHeight),
    };
}

static Vector2 cobaltPoseToVector2(Pose pose)
{
    return cobaltPointToVector2({pose.x, pose.y});
}

static Rectangle cobaltRectToRectangle(Rect rect)
{
    Vector2 vector = cobaltPoseToVector2({rect.x, rect.y});
    return {vector.x, vector.y, cobaltDistToDist(rect.w),
            cobaltDistToDist(rect.h)};
}

static void DrawRectangleLinesPro(Rectangle rect, Vector2 origin, float theta,
                                  float thick, Color color)
{
    rlPushMatrix();
    {
        rlTranslatef(rect.x, rect.y, 0.0f);
        rlRotatef(theta, 0.0f, 0.0f, 1.0f);
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

    for (const auto &object : field.objects)
    {
        drawObject(*object);
    }
}

void Renderer::drawRobot(const Robot &robot)
{
    Rectangle rect = cobaltRectToRectangle(robot.rect);
    Vector2 origin = {rect.width / 2.0f, rect.height / 2.0f};
    DrawRectanglePro(rect, origin, robot.theta * RAD2DEG, WHITE);

    for (const auto &device : robot.devices)
    {
        DeviceContext deviceContext = {rect, robot.theta};
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
    Rectangle dest = cobaltRectToRectangle(pin.rect);
    Rectangle source = {static_cast<float>(pin.kind) * 467.0f, 0.0f, 467.0f,
                        static_cast<float>(pinsTexture.height)};

    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
    DrawTexturePro(pinsTexture, source, dest, origin, pin.theta * RAD2DEG,
                   WHITE);
}

void Renderer::drawVerticalPin(const VerticalPin &pin)
{
    Vector2 vec = cobaltPointToVector2(pin.point);
    Color color = WHITE;
    switch (pin.kind)
    {
    case Pin::Kind::YellowYellow:
        color = fieldYellow;
        break;
    case Pin::Kind::RedYellow:
        color = pin.up ? fieldRed : fieldYellow;
        break;
    case Pin::Kind::BlueYellow:
        color = pin.up ? fieldBlue : fieldYellow;
        break;
    case Pin::Kind::RedBlue:
        color = pin.up ? fieldRed : fieldBlue;
        break;
    }
    DrawCircleV(vec, cobaltDistToDist(pin.radius), color);
}

void Renderer::drawCup(const Cup &cup)
{
    Vector2 vec = cobaltPointToVector2(cup.point);
    Color color = cup.up ? LIGHTGRAY : DARKGRAY;
    DrawCircleV(vec, cobaltDistToDist(cup.radius), color);
}

void Renderer::drawMotor(const Motor &motor, const DeviceContext &context)
{
    Rectangle dest = {
        cobaltDistToDist(static_cast<cobalt::Dist>(motor.rect.x)),
        cobaltDistToDist(static_cast<cobalt::Dist>(motor.rect.y)),
        cobaltDistToDist(static_cast<cobalt::Dist>(motor.rect.w)),
        cobaltDistToDist(static_cast<cobalt::Dist>(motor.rect.h)),
    };

    dest.x += context.rect.x;
    dest.y += context.rect.y;
    float theta = motor.theta + context.theta;

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

    DrawRectangleLinesPro(dest, origin, theta * RAD2DEG, 1.0f, color);
}

void Renderer::drawDevice(const Device &device, const DeviceContext &context)
{
    device.draw(*this, context);
}

} // namespace cobalt
