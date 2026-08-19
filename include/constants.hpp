#pragma once
#include "utils.hpp"

namespace cobalt
{

constexpr int fps = 60;

constexpr float fieldSize = inchesToMeters(140.0f); // meters
constexpr float halfFieldSize = fieldSize / 2.0f;   // meters
constexpr float fieldMin = -halfFieldSize;          // meters
constexpr float fieldMax = halfFieldSize;           // meters

constexpr float wallSize = inchesToMeters(2.0f);             // meters
constexpr float fieldWallSize = fieldSize + wallSize * 2.0f; // meters
constexpr float halfFieldWallSize = fieldWallSize / 2.0f;    // meters
constexpr float fieldWallMin = -halfFieldWallSize;           // meters
constexpr float fieldWallMax = halfFieldWallSize;            // meters

} // namespace cobalt
