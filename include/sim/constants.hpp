#pragma once
#include "units.hpp"
#include <numbers>

namespace bromine
{

constexpr float fixedDt = 1.0f / 240.0f;

constexpr float gravity = 9.81f;

constexpr float degreesToRadians = std::numbers::pi_v<float> / 180.0f;
constexpr float radiansToDegrees = 180.0f / std::numbers::pi_v<float>;
constexpr float radiansToRotations = 1.0f / (2.0f * std::numbers::pi_v<float>);
constexpr float rotationsToRadians = (2.0f * std::numbers::pi_v<float>) / 1.0f;
constexpr float degreesToRotations = 1.0f / 360.0f;
constexpr float rotationsToDegrees = 360.0f / 1.0f;

constexpr InputDist inputFieldSize = 140.0f;
constexpr InputDist inputHalfFieldSize = inputFieldSize / 2.0f;
constexpr InputDist inputFieldMin = -inputHalfFieldSize;
constexpr InputDist inputFieldMax = inputHalfFieldSize;

constexpr InputDist inputWallSize = 2.0f;
constexpr InputDist inputFieldWallSize = inputFieldSize + inputWallSize * 2.0f;
constexpr InputDist inputHalfFieldWallSize = inputFieldWallSize / 2.0f;
constexpr InputDist inputFieldWallMin = -inputHalfFieldWallSize;
constexpr InputDist inputFieldWallMax = inputHalfFieldWallSize;

constexpr PhysicsDist physicsFieldSize = inputDistToPhysicsDist(inputFieldSize);
constexpr PhysicsDist physicsHalfFieldSize =
    inputDistToPhysicsDist(inputHalfFieldSize);
constexpr PhysicsDist physicsFieldMin = inputDistToPhysicsDist(inputFieldMin);
constexpr PhysicsDist physicsFieldMax = inputDistToPhysicsDist(inputFieldMax);

constexpr PhysicsDist physicsWallSize = inputDistToPhysicsDist(inputWallSize);
constexpr PhysicsDist physicsFieldWallSize =
    inputDistToPhysicsDist(inputFieldWallSize);
constexpr PhysicsDist physicsHalfFieldWallSize =
    inputDistToPhysicsDist(inputHalfFieldWallSize);
constexpr PhysicsDist physicsFieldWallMin =
    inputDistToPhysicsDist(inputFieldWallMin);
constexpr PhysicsDist physicsFieldWallMax =
    inputDistToPhysicsDist(inputFieldWallMax);

} // namespace bromine
