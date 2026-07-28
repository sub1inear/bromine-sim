#pragma once

namespace cobalt
{

constexpr float fieldSize = 140.0f; // In in.
constexpr float halfFieldSize = fieldSize / 2.0f;
constexpr float fieldMin = -halfFieldSize;
constexpr float fieldMax = halfFieldSize;

constexpr float wallSize = 2.0f; // In in.
constexpr float fieldWallSize = fieldSize + wallSize * 2.0f;
constexpr float halfFieldWallSize = fieldWallSize / 2.0f;
constexpr float fieldWallMin = -halfFieldWallSize;
constexpr float fieldWallMax = halfFieldWallSize;

constexpr float gravity = 360.0886f; // In in / s^2.

constexpr float linearForce = 5000.0f;    // In lbs * in / s^2.
constexpr float rotationalForce = 500.0f; // In lbs * in / s^2.

constexpr float maxLinearVelocity = 50.0f;
constexpr float maxRotationalVelocity = 10.0f;

constexpr float linearFriction = 0.9f;
constexpr float rotationalFriction = 0.9f;

constexpr float epsilon = 0.1f;

} // namespace cobalt
