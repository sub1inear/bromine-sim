#include "robot.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <print>
#include <raylib.h>

namespace cobalt
{

constexpr float gravity = 360.0886f; // In in / s^2.

constexpr float linearForce = 5000.0f;    // In lbs * in / s^2.
constexpr float rotationalForce = 500.0f; // In lbs * in / s^2.

constexpr float maxLinearVelocity = 50.0f;
constexpr float maxRotationalVelocity = 10.0f;

constexpr float linearFriction = 0.9f;
constexpr float rotationalFriction = 0.9f;

constexpr float epsilon = 0.1f;

float wrapTheta(float theta)
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

template <typename T> T sign(T v)
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
    rect.y += velocity.y * dt;

    rect.x = std::clamp(rect.x, -72.0f, 72.0f);
    rect.y = std::clamp(rect.y, -72.0f, 72.0f);

    theta += velocity.theta * dt;
    theta = wrapTheta(theta);
}

} // namespace cobalt
