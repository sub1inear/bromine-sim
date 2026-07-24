#include "robot.hpp"
#include <cmath>
#include <iostream>
#include <numbers>
#include <raylib.h>

namespace cobalt
{

constexpr float gravity = 360.0886f; // in / s^2

constexpr Field::Dist linearForce = 500.0f;
constexpr Field::Dist rotationalForce = 50.0f;

constexpr float friction = 1.0f;
constexpr float mass = 16.0f; // lbm

constexpr float minVelocity = 0.1f;
constexpr float minSpeed = 0.1f;

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
    Field::Pose driveForce{0.0f, 0.0f};
    bool driving = false;

    if (IsKeyDown(KEY_LEFT))
    {
        driveForce.theta -= rotationalForce;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        driveForce.theta += rotationalForce;
    }
    if (IsKeyDown(KEY_UP))
    {
        driveForce.x -= std::cos(theta) * linearForce;
        driveForce.y -= std::sin(theta) * linearForce;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        driveForce.x += std::cos(theta) * linearForce;
        driveForce.y += std::sin(theta) * linearForce;
    }

    Field::Pose force = driveForce;

    Field::Dist speed =
        std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    // Apply friction if `speed` is greater than an epsilon.
    if (speed >= minSpeed)
    {
        force.x -= gravity * friction * velocity.x / speed;
        force.y -= gravity * friction * velocity.y / speed;
    }

    // F = ma, so a = F / m
    Field::Pose acceleration{force.x / mass, force.y / mass,
                             force.theta / mass};

    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;
    velocity.theta += acceleration.theta * dt;

    rect.x += velocity.x * dt;
    rect.y += velocity.y * dt;
    theta += velocity.theta * dt;
}

} // namespace cobalt
