#include "robot.hpp"
#include "constants.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <print>
#include <raylib.h>

namespace cobalt
{

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

template <typename T> static T sign(T v)
{
    if (v > 0)
    {
        return static_cast<T>(1);
    }
    else if (v < 0)
    {
        return static_cast<T>(-1);
    }
    else
    {
        return static_cast<T>(0);
    }
}

void Robot::update(float dt)
{
    Pose force = {0.0f, 0.0f};
    bool driving = false;

    if (IsKeyDown(KEY_LEFT))
    {
        driving = true;
        force.theta -= rotationalForce;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        driving = true;
        force.theta += rotationalForce;
    }
    if (IsKeyDown(KEY_UP))
    {
        driving = true;
        force.x -= std::sin(theta) * linearForce;
        force.y -= std::cos(theta) * linearForce;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        driving = true;
        force.x += std::sin(theta) * linearForce;
        force.y += std::cos(theta) * linearForce;
    }
    // F = m * a, so a = F / m.
    Pose acceleration = force / mass;
    velocity += acceleration * dt;

    // Apply (approximate) friction.
    velocity.x *= linearFriction;
    velocity.y *= linearFriction;
    velocity.theta *= rotationalFriction;

    // Apply maximum velocities.
    float linearVelocity =
        std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    if (linearVelocity > maxLinearVelocity)
    {
        velocity.x = velocity.x / linearVelocity * maxLinearVelocity;
        velocity.y = velocity.y / linearVelocity * maxLinearVelocity;
    }

    velocity.theta = std::clamp(velocity.theta, -maxRotationalVelocity,
                                maxRotationalVelocity);

    // Avoid denormalization.
    if (!driving)
    {
        if (std::abs(velocity.x) < epsilon)
        {
            velocity.x = 0.0f;
        }
        if (std::abs(velocity.y) < epsilon)
        {
            velocity.y = 0.0f;
        }
        if (std::abs(velocity.theta) < epsilon)
        {
            velocity.theta = 0.0f;
        }
    }

    rect.x += velocity.x * dt;
    if (collidesWithWall())
    {
        rect.x -= velocity.x * dt;
    }

    rect.y += velocity.y * dt;
    if (collidesWithWall())
    {
        rect.y -= velocity.y * dt;
    }

    theta += velocity.theta * dt;
    theta = wrapTheta(theta);
    if (collidesWithWall())
    {
        theta -= velocity.theta * dt;
    }
}
bool Robot::collidesWithWall()
{
    float sinTheta = std::sin(theta);
    float cosTheta = std::cos(theta);

    float halfW = rect.w / 2.0f;
    float halfH = rect.h / 2.0f;

    float centerX = rect.x;
    float centerY = rect.y;

    Point localCorners[4] = {
        {-halfW, -halfH},
        {halfW, -halfH},
        {halfW, halfH},
        {-halfW, halfH},
    };
    for (const Point &localCorner : localCorners)
    {
        Point corner = {
            rect.x + localCorner.x * sinTheta - localCorner.y * cosTheta,
            rect.y + localCorner.x * cosTheta + localCorner.y * sinTheta,
        };
        if (corner.x < fieldMin || corner.x > fieldMax || corner.y < fieldMin ||
            corner.y > fieldMax)
        {
            return true;
        }
    }
    return false;
}
} // namespace cobalt
