#include "units.hpp"
#include "constants.hpp"
#include <cassert>

namespace bromine
{

static void setBodyMass(b2BodyId bodyId, InputMass mass)
{
    PhysicsMass newMass = inputMassToPhysicsMass(mass);

    b2MassData oldMassData = b2Body_GetMassData(bodyId);
    PhysicsMass oldMass = oldMassData.mass;

    // Skip static bodies (mass of 0.0f).
    if (oldMass == 0.0f)
    {
        return;
    }

    float scalingFactor = newMass / oldMass;
    b2MassData newMassData = {newMass, oldMassData.center,
                              oldMassData.rotationalInertia * scalingFactor};
}

PhysicsAngle inputAngleToPhysicsAngle(InputAngle angle)
{
    return b2MakeRot(-angle * DEG2RAD);
}

PhysicsPoint inputPointToPhysicsPoint(InputPoint point)
{
    return {inputDistToPhysicsDist(point.x), inputDistToPhysicsDist(point.y)};
}

PhysicsPose inputPoseToPhysicsPose(InputPose pose)
{
    return {{inputDistToPhysicsDist(pose.x), inputDistToPhysicsDist(pose.y)},
            inputAngleToPhysicsAngle(pose.angle)};
}

PhysicsCircle inputCircleToPhysicsCircle(InputCircle circle)
{
    return {inputPoseToPhysicsPose({circle.x, circle.y, circle.angle}),
            inputDistToPhysicsDist(circle.radius),
            inputMassToPhysicsMass(circle.mass)};
}

PhysicsIdCircle inputCircleToPhysicsIdCircle(InputCircle circle,
                                             b2WorldId worldId,
                                             b2BodyDef &bodyDef,
                                             b2ShapeDef &shapeDef)
{
    PhysicsIdCircle out;

    bodyDef.position = inputPointToPhysicsPoint({circle.x, circle.y});
    bodyDef.rotation = inputAngleToPhysicsAngle(circle.angle);

    out.bodyId = b2CreateBody(worldId, &bodyDef);
    out.radius = inputDistToPhysicsDist(circle.radius);

    b2Circle circleDef = {{0.0f, 0.0f}, out.radius};
    b2ShapeId shapeId = b2CreateCircleShape(out.bodyId, &shapeDef, &circleDef);

    setBodyMass(out.bodyId, circle.mass);
    return out;
}

PhysicsRect inputRectToPhysicsRect(InputRect rect)
{
    return {inputPoseToPhysicsPose({rect.x, rect.y, rect.angle}),
            inputDistToPhysicsDist(rect.width),
            inputDistToPhysicsDist(rect.height),
            inputMassToPhysicsMass(rect.mass)};
}

PhysicsIdRect inputRectToPhysicsIdRect(InputRect rect, b2WorldId worldId,
                                       b2BodyDef &bodyDef, b2ShapeDef &shapeDef)
{
    PhysicsIdRect out;

    bodyDef.position = inputPointToPhysicsPoint({rect.x, rect.y});
    bodyDef.rotation = inputAngleToPhysicsAngle(rect.angle);

    out.bodyId = b2CreateBody(worldId, &bodyDef);
    out.width = inputDistToPhysicsDist(rect.width);
    out.height = inputDistToPhysicsDist(rect.height);

    b2Polygon boxDef = b2MakeBox(out.width * 0.5f, out.height * 0.5f);
    b2ShapeId shapeId = b2CreatePolygonShape(out.bodyId, &shapeDef, &boxDef);

    setBodyMass(out.bodyId, rect.mass);
    return out;
}

ScreenDist physicsDistToScreenDist(PhysicsDist dist)
{
    assert(GetScreenWidth() == GetScreenHeight());
    float screenWidth = static_cast<float>(GetScreenWidth());

    return remapDist(dist, physicsFieldWallMin, physicsFieldWallMax, 0.0f,
                     screenWidth);
}

ScreenAngle physicsAngleToScreenAngle(PhysicsAngle angle)
{
    return -b2Rot_GetAngle(angle) * RAD2DEG;
}

ScreenPoint physicsPointToScreenPoint(PhysicsPoint point)
{
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());
    return {
        remapValue(point.x, physicsFieldWallMin, physicsFieldWallMax, 0.0f,
                   screenWidth),
        remapValue(-point.y, physicsFieldWallMin, physicsFieldWallMax, 0.0f,
                   screenHeight),
    };
}

ScreenPose physicsPoseToScreenPose(PhysicsPose pose)
{
    ScreenPoint point = physicsPointToScreenPoint({pose.p.x, pose.p.y});
    return {point.x, point.y, physicsAngleToScreenAngle(pose.q)};
}

ScreenCircle physicsCircleToScreenCircle(PhysicsCircle circle)
{
    ScreenPose pose = physicsPoseToScreenPose(circle.pose);
    return {pose.x, pose.y, pose.angle, physicsDistToScreenDist(circle.radius)};
}

ScreenCircle physicsIdCircleToScreenCircle(PhysicsIdCircle circle)
{
    PhysicsPose pose = b2Body_GetTransform(circle.bodyId);
    return physicsCircleToScreenCircle({pose, circle.radius});
}

ScreenRect physicsRectToScreenRect(PhysicsRect rect)
{
    ScreenPose pose = physicsPoseToScreenPose(rect.pose);
    return {pose.x, pose.y, pose.angle, physicsDistToScreenDist(rect.width),
            physicsDistToScreenDist(rect.height)};
}

ScreenRect physicsIdRectToScreenRect(PhysicsIdRect rect)
{
    PhysicsPose pose = b2Body_GetTransform(rect.bodyId);
    return physicsRectToScreenRect({pose, rect.width, rect.height});
}
} // namespace bromine
