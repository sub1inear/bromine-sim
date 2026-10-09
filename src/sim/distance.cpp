#include "distance.hpp"
#include "categories.hpp"
#include "device.hpp"
#include "renderer.hpp"
#include "robot.hpp"
#include "units.hpp"
#include <box2d/box2d.h>

namespace bromine
{
// See https://www.vexrobotics.com/276-4852.html.
constexpr PhysicsDist maxDistance = 2.0f; // meters
constexpr int32_t outOfRangeDistance = 9999;

Distance::Distance(const json &j, Robot &robot)
    : Device(j, robot), port(expectJsonPort(j))
{
    // Measured on https://www.vexrobotics.com/276-4852.html.
    rect.width = inputDistToPhysicsDist(0.86406653543f);
    rect.height = inputDistToPhysicsDist(0.69637047244f);
}

void Distance::preUpdate() {}

void Distance::postUpdate()
{
    b2WorldId worldId = b2Body_GetWorld(robot.rect.bodyId);

    // Distance comes from the center-front point of the distance sensor.
    PhysicsRect worldRect = getWorldRect();
    lineStart =
        b2TransformPoint(worldRect.pose, {0.0f, worldRect.height / 2.0f});

    // Filter to only hit the walls.
    b2QueryFilter filter = b2DefaultQueryFilter();
    filter.maskBits = std::to_underlying(Categories::Wall);

    // Box2D combines the angle and the max distance into one vector.
    PhysicsPoint translation =
        b2MulSV(maxDistance, b2Rot_GetYAxis(worldRect.pose.q));

    b2RayResult result =
        b2World_CastRayClosest(worldId, lineStart, translation, filter);

    // Box2D only provides the hit point if the ray actually hits an object.
    // Otherwise, just set the end point and distances to their maxes.
    if (result.hit)
    {
        lineEnd = result.point;
        PhysicsPoint delta = b2Sub(lineEnd, lineStart);
        float distance = b2Length(delta);
        sdk.distance = static_cast<int32_t>(distance * 1000.0f); // m to mm.
    }
    else
    {
        lineEnd = b2Add(lineStart, translation);
        sdk.distance = outOfRangeDistance;
    }
}

void Distance::draw(Renderer &renderer) const { renderer.drawDistance(*this); }

bool Distance::isOutOfRange() const
{
    return sdk.distance == outOfRangeDistance;
}
} // namespace bromine
