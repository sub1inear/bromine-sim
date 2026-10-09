#include "wheel.hpp"
#include "constants.hpp"
#include "robot.hpp"
#include "utils.hpp"
#include <algorithm>

namespace bromine
{
Wheel::Wheel(const json &j, Robot &robot)
    : Device(j, robot), id(expectJsonKey<uint32_t>(j, "id"))
{
    std::string_view subtypeStr = expectJsonKey<std::string_view>(j, "subtype");
    if (subtypeStr == "omni")
    {
        subtype = Subtype::Omni;
        linearFriction = 0.01f;
        lateralFriction = 0.15f;
    }
    else if (subtypeStr == "traction")
    {
        subtype = Subtype::Traction;
        linearFriction = 0.15f;
        lateralFriction = 1.0f;
    }
    else if (subtypeStr == "flex")
    {
        subtype = Subtype::Flex;
        linearFriction = 0.15f;
        lateralFriction = 1.0f;
    }
    else
    {
        error("unknown wheel subtype `{}`, expected one of: `omni`, "
              "`traction`, `flex`",
              subtypeStr);
    }
    rect.width = inputDistToPhysicsDist(expectJsonKey<float>(j, "width"));
    rect.height = inputDistToPhysicsDist(expectJsonKey<float>(j, "diameter"));
}

void Wheel::preUpdate()
{
    // Apply friction.
    PhysicsRect worldRect = getWorldRect();

    // Get the forward/right vectors.
    PhysicsPoint forward = b2RotateVector(worldRect.pose.q, {0.0f, 1.0f});
    PhysicsPoint right = b2RightPerp(forward);

    // Get the velocity of the wheel.
    PhysicsPoint velocity =
        b2Body_GetWorldPointVelocity(robot.rect.bodyId, worldRect.pose.p);

    // Each wheel has only a proportion of the total mass of the robot.
    float massShare = b2Body_GetMass(robot.rect.bodyId) / robot.wheelCount;
    // The normal force is the weight supported by this wheel.
    float normalForce = massShare * gravity;

    float maxLateral = lateralFriction * normalForce;

    // Calculate the lateral force needed to resist lateral movement
    // with the amount that the velocity overlaps with the right vector.
    // The final force is calculated with F = m * dv / dt
    // (from Newton's second law F = ma, with a = dv / dt).
    float lateralForce = std::clamp(
        -b2Dot(velocity, right) * massShare / fixedDt, -maxLateral, maxLateral);

    // Same except for linear force.
    float maxLinear = linearFriction * normalForce;
    float linearForce = std::clamp(
        -b2Dot(velocity, forward) * massShare / fixedDt, -maxLinear, maxLinear);

    // Multiply the scalars by the unit vectors to find the final force.
    PhysicsPoint total =
        b2Add(b2MulSV(linearForce, forward), b2MulSV(lateralForce, right));
    b2Body_ApplyForce(robot.rect.bodyId, total, worldRect.pose.p, true);
}

void Wheel::postUpdate() {}

void Wheel::draw(Renderer &renderer) const { renderer.drawWheel(*this); }

} // namespace bromine
