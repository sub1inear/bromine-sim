#pragma once
#include "units.hpp"

namespace bromine
{

constexpr int fps = 60;

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
