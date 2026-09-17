#include "renderer.hpp"
#include "constants.hpp"
#include "cup.hpp"
#include "motor.hpp"
#include "pin.hpp"
#include "sim.hpp"
#include "units.hpp"
#include "utils.hpp"
#include "wheel.hpp"
#include <array>
#include <cassert>
#include <cmath>
#include <raymath.h>
#include <rlgl.h>

namespace bromine
{

constexpr Color fieldRed = {209, 46, 81, 255};
constexpr Color fieldBlue = {87, 156, 189, 255};
constexpr Color fieldYellow = {199, 191, 120, 255};

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

Renderer::Renderer(b2WorldId worldId) : worldId(worldId), debug(false)
{
    fieldTexture = LoadTexture("assets/field.png");
    pinsTexture = LoadTexture("assets/pins.png");
    GenTextureMipmaps(&pinsTexture);
    SetTextureFilter(pinsTexture, TEXTURE_FILTER_TRILINEAR);
}

Renderer::~Renderer() { UnloadTexture(fieldTexture); }

void Renderer::draw(const Sim &sim)
{
    if (IsKeyPressed(KEY_D))
    {
        debug = !debug;
    }
    BeginDrawing();
    {
        drawField(sim.field);
        drawRobot(sim.robot);
        if (debug)
        {
            drawDebug();
        }
    }
    EndDrawing();
}

void Renderer::drawDebug()
{
    struct b2DebugDraw debugDraw = {};
    debugDraw.DrawSolidPolygonFcn =
        [](b2Transform transform, const b2Vec2 *vertices, int vertexCount,
           float radius, b2HexColor color, void *context)
    {
        std::array<ScreenPoint, 32> points;
        assert(points.size() >= vertexCount);
        for (int i = 0; i < vertexCount; i++)
        {
            points.at(i) = physicsPointToScreenPoint(
                b2TransformPoint(transform, vertices[i]));
        }
        for (int i = 0; i < vertexCount; i++)
        {
            ScreenPoint &point = points[i];
            ScreenPoint &nextPoint = points[(i + 1) % vertexCount];
            DrawLineV(point, nextPoint, MAGENTA);
        }
    };
    debugDraw.DrawSolidCircleFcn =
        [](b2Transform transform, float radius, b2HexColor color, void *context)
    {
        PhysicsCircle physicsCircle = {transform, radius};
        ScreenCircle screenCircle = physicsCircleToScreenCircle(physicsCircle);
        DrawCircleLinesV({screenCircle.x, screenCircle.y}, screenCircle.radius,
                         MAGENTA);
    };
    debugDraw.drawShapes = true;
    b2World_Draw(worldId, &debugDraw);
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
    ScreenRect rect = physicsIdRectToScreenRect(robot.rect);

    Vector2 origin = {rect.width / 2.0f, rect.height / 2.0f};
    DrawRectanglePro({rect.x, rect.y, rect.width, rect.height}, origin,
                     rect.angle, WHITE);

    for (const auto &device : robot.devices)
    {
        drawDevice(*device, b2Body_GetTransform(robot.rect.bodyId));
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
    ScreenRect dest = physicsIdRectToScreenRect(pin.rect);
    Rectangle source = {static_cast<float>(pin.type) * 467.0f, 0.0f, 467.0f,
                        static_cast<float>(pinsTexture.height)};

    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
    DrawTexturePro(pinsTexture, source,
                   {dest.x, dest.y, dest.width, dest.height}, origin,
                   dest.angle, WHITE);
}

void Renderer::drawVerticalPin(const VerticalPin &pin)
{
    ScreenCircle circle = physicsIdCircleToScreenCircle(pin.circle);
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
    DrawCircleV({circle.x, circle.y}, circle.radius, color);
}

void Renderer::drawCup(const Cup &cup)
{
    ScreenCircle circle = physicsIdCircleToScreenCircle(cup.circle);
    Color color = cup.up ? LIGHTGRAY : DARKGRAY;
    DrawCircleV({circle.x, circle.y}, circle.radius, color);
}

void Renderer::drawMotor(const Motor &motor, PhysicsPose parentPose)
{
    PhysicsRect physicsRect = motor.getWorldRect(parentPose);
    ScreenRect screenRect = physicsRectToScreenRect(physicsRect);

    Vector2 origin = {screenRect.width / 2.0f, screenRect.height / 2.0f};

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

    DrawRectangleLinesPro(
        {screenRect.x, screenRect.y, screenRect.width, screenRect.height},
        origin, screenRect.angle, 1.5f, color);
}

void Renderer::drawWheel(const Wheel &wheel, PhysicsPose parentPose)
{
    PhysicsRect physicsRect = wheel.getWorldRect(parentPose);
    ScreenRect screenRect = physicsRectToScreenRect(physicsRect);

    Vector2 origin = {screenRect.width / 2.0f, screenRect.height / 2.0f};

    DrawRectangleLinesPro(
        {screenRect.x, screenRect.y, screenRect.width, screenRect.height},
        origin, screenRect.angle, 1.5f, PURPLE);
}

void Renderer::drawDevice(const Device &device, PhysicsPose parentPose)
{
    device.draw(*this, parentPose);
}

} // namespace bromine
