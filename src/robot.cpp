#include "robot.hpp"
#include <cmath>
#include <numbers>
#include <raylib.h>

namespace cobalt
{
constexpr float movementSpeed = 5.0f;
constexpr float turnSpeed = 1.0f;

static float wrapTheta(float theta)
{
    constexpr float twoPi = 2.0f * std::numbers::pi_v<float>;

    while (theta > twoPi)
    {
        theta -= twoPi;
    }
    while (theta < twoPi)
    {
        theta += twoPi;
    }

    return theta;
}

void Robot::update(float dt)
{
    if (IsKeyDown(KEY_LEFT))
    {
        theta -= turnSpeed * dt;
        theta = wrapTheta(theta);
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        theta += turnSpeed * dt;
        theta = wrapTheta(theta);
    }
    if (IsKeyDown(KEY_UP))
    {
        float coef = movementSpeed * dt;

        rect.x -= coef * std::cos(theta);
        rect.y -= coef * std::sin(theta);
    }
    if (IsKeyDown(KEY_DOWN))
    {
        float coef = movementSpeed * dt;

        rect.x += coef * std::cos(theta);
        rect.y += coef * std::sin(theta);
    }
}
} // namespace cobalt
