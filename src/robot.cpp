#include "robot.hpp"
#include "constants.hpp"
#include "motor.hpp"
#include "utils.hpp"
#include "wheel.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <numbers>
#include <print>

namespace cobalt
{

static float wrapTheta(float theta)
{
    constexpr float twoPi = 2.0f * std::numbers::pi_v<float>;

    while (theta > twoPi)
    {
        theta -= twoPi;
    }
    while (theta < 0.0f)
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

Robot::Robot() : velocity(0.0f, 0.0f, 0.0f)
{
    std::ifstream file("assets/robot.json");
    json j;
    try
    {
        j = json::parse(file, nullptr, true, true);
    }
    catch (const json::parse_error &e)
    {
        error("{}", e.what());
    }

    uint64_t version = expectJsonKey<uint64_t>(j, "version");

    if (version != 1)
    {
        error("JSON version mismatch: `{}` vs. `{}`", version, 1);
    }

    rect.x = 0.0f;
    rect.y = 0.0f;
    rect.theta = 0.0f;
    rect.w = expectJsonKey<float>(j, "width");
    rect.h = expectJsonKey<float>(j, "height");
    mass = expectJsonKey<float>(j, "mass");

    std::vector<json> vec = expectJsonKey<std::vector<json>>(j, "devices");

    for (const auto &elem : vec)
    {
        std::string_view type = expectJsonKey<std::string_view>(elem, "type");
        if (type == "motor")
        {
            devices.push_back(std::make_unique<Motor>(elem));
        }
        else if (type == "wheel")
        {
            devices.push_back(std::make_unique<Wheel>(elem));
        }
        else
        {
            error("unknown device type `{}`", type);
        }
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
        force.x += std::sin(rect.theta) * linearForce;
        force.y += std::cos(rect.theta) * linearForce;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        driving = true;
        force.x -= std::sin(rect.theta) * linearForce;
        force.y -= std::cos(rect.theta) * linearForce;
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
    rect += velocity * dt;
    rect.theta = wrapTheta(rect.theta);

    float halfWidth = rect.w / 2.0f;
    float halfHeight = rect.h / 2.0f;

    float cosTheta = std::abs(std::cos(rect.theta));
    float sinTheta = std::abs(std::sin(rect.theta));

    float eHalfWidth = halfWidth * cosTheta + halfHeight * sinTheta;
    float eHalfHeight = halfWidth * sinTheta + halfWidth * cosTheta;

    rect.x = std::clamp(rect.x, fieldMin + eHalfWidth, fieldMax - eHalfWidth);
    rect.y = std::clamp(rect.y, fieldMin + eHalfHeight, fieldMax - eHalfHeight);

    for (const auto &device : devices)
    {
        device->update(dt);
    }
}
} // namespace cobalt
