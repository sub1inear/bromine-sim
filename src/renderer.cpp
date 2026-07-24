#include "renderer.hpp"
#include "sim.hpp"
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

static float fieldDistToDist(Field::Dist dist)
{
    assert(GetScreenWidth() == GetScreenHeight());
    float screenWidth = static_cast<float>(GetScreenWidth());

    return remapDist(dist, fieldMin, fieldMax, 0.0f, screenWidth);
}

static Vector2 fieldVector2ToVector2(Field::Vector2 pos)
{
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    return Vector2{remapValue(pos.x, fieldMin, fieldMax, 0.0f, screenWidth),
                   remapValue(pos.y, fieldMin, fieldMax, 0.0f, screenHeight)};
}

static Rectangle fieldRectToRectangle(Field::Rect rect)
{
    Vector2 vec = fieldVector2ToVector2(Field::Vector2{rect.x, rect.y});

    Rectangle newRect{vec.x, vec.y, fieldDistToDist(rect.w),
                      fieldDistToDist(rect.h)};

    newRect.x -= newRect.width / 2.0f;
    newRect.y -= newRect.height / 2.0f;

    return newRect;
}

Renderer::Renderer()
{
    Image fieldImage = LoadImage("assets/field.png");
    fieldTexture = LoadTextureFromImage(fieldImage);
    UnloadImage(fieldImage);
}

Renderer::~Renderer() { UnloadTexture(fieldTexture); }

void Renderer::draw(const Sim &sim)
{

    BeginDrawing();
    {
        Renderer::drawField(sim.field);
        Renderer::drawRobot(sim.robot);
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
}

void Renderer::drawRobot(const Robot &robot)
{
    Rectangle rect = fieldRectToRectangle(robot.rect);
    rect.x += rect.width / 2.0f;
    rect.y += rect.height / 2.0f;
    Vector2 origin = {rect.width / 2.0f, rect.height / 2.0f};
    std::cout << robot.theta * RAD2DEG << '\n';
    DrawRectanglePro(rect, origin, robot.theta * RAD2DEG, WHITE);
}

} // namespace cobalt
