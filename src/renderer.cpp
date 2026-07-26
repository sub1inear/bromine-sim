#include "renderer.hpp"
#include "sim.hpp"
#include "types.hpp"
#include <cassert>
#include <cstdio>
#include <iostream>

namespace cobalt
{

constexpr float fieldSize = 144.0f;
constexpr float halfFieldSize = fieldSize / 2.0f;
constexpr float fieldMin = -halfFieldSize;
constexpr float fieldMax = halfFieldSize;

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

    return remapDist(dist, fieldMin, fieldMax, 0.0f, screenWidth);
}

static Vector2 cobaltPoseToVector2(Pose pose)
{
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    return {remapValue(pose.x, fieldMin, fieldMax, 0.0f, screenWidth),
            remapValue(-pose.y, fieldMin, fieldMax, 0.0f, screenHeight)};
}

static Rectangle cobaltRectToRectangle(Rect rect)
{
    Vector2 vector = cobaltPoseToVector2({rect.x, rect.y});
    return {vector.x, vector.y, cobaltDistToDist(rect.w),
            cobaltDistToDist(rect.h)};
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

    for (const Pin &pin : field.pins)
    {
        drawPin(pin);
    }
}

void Renderer::drawRobot(const Robot &robot)
{
    Rectangle rect = cobaltRectToRectangle(robot.rect);
    Vector2 origin = {rect.width / 2.0f, rect.height / 2.0f};
    DrawRectanglePro(rect, origin, robot.theta * RAD2DEG, WHITE);
}

#include <iostream>

void Renderer::drawPin(const Pin &pin)
{
    Rectangle dest = cobaltRectToRectangle(pin.rect);
    Rectangle source = {static_cast<float>(pin.kind) * 467.0f, 0.0f, 467.0f,
                        static_cast<float>(pinsTexture.height)};

    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
    DrawTexturePro(pinsTexture, source, dest, origin, pin.theta * RAD2DEG,
                   WHITE);
}

} // namespace cobalt
