/*
bromine-sim has an extremely complicated unit system.
Input units -> (0, 0) at center, inches, lbs, degrees, CW, 0-up.
Physics units -> (0, 0) at center, meters, kilograms, radians, CCW, 0-up.
Screen units -> (0, 0) at top left, pixels, degrees, CW, 0-up.

The reasons for this are:
Vex/the Vex community standardized the input units.
It's much easier to input in inches, lbs, degrees, CW, etc. because those are
returned by external tooling and Vex's own measurements.

Box2D requires the physics units as the tolerances are set specifically for
these units.

Raylib requires the screen units to actually draw things to the
screen.

At startup, input units are converted to physics units.
When drawing, physics units are converted to screen units.
**Object constructors take input units.**
*/

// TODO: Write this with namespaces instead of prefixes.

#pragma once
#include <algorithm>
#include <array>
#include <box2d/box2d.h>
#include <raylib.h>
#include <utility>
#include <vector>

namespace bromine
{

using InputDist = float;  // inches
using InputMass = float;  // lbs
using InputAngle = float; // degrees

struct InputPoint
{
    InputDist x;
    InputDist y;
};

struct InputPose
{
    InputDist x;
    InputDist y;
    InputAngle angle;
};

struct InputCircle
{
    InputDist x;
    InputDist y;
    InputAngle angle;
    InputDist radius;
    InputMass mass;
};

struct InputRect
{
    InputDist x;
    InputDist y;
    InputAngle angle;
    InputDist width;
    InputDist height;
    InputMass mass;
};

struct InputPolygon
{
    InputDist x;
    InputDist y;
    InputAngle angle;
    InputDist radius; // Normally 0.0f.
    std::vector<InputPoint> verticies;
    InputMass mass;
};

using PhysicsDist = float;  // meters
using PhysicsMass = float;  // kilograms
using PhysicsAngle = b2Rot; // radians (cosine/sine pair)
using PhysicsPoint = b2Vec2;
using PhysicsPose = b2Transform;

struct PhysicsCircle
{
    PhysicsPose pose;
    PhysicsDist radius;
    PhysicsMass mass;
};

struct PhysicsIdCircle
{
    b2BodyId bodyId;
    PhysicsDist radius;
};

struct PhysicsRect
{
    PhysicsPose pose;
    PhysicsDist width;
    PhysicsDist height;
    PhysicsMass mass;
};

struct PhysicsIdRect
{
    b2BodyId bodyId;
    PhysicsDist width;
    PhysicsDist height;
};

using PhysicsIdPolygon = b2BodyId;

using ScreenDist = float;  // pixels
using ScreenAngle = float; // degrees
using ScreenPoint = Vector2;

struct ScreenPose
{
    ScreenDist x;
    ScreenDist y;
    ScreenAngle angle;
};

struct ScreenCircle
{
    ScreenDist x;
    ScreenDist y;
    ScreenAngle angle;
    ScreenDist radius;
};

struct ScreenRect
{
    ScreenDist x;
    ScreenDist y;
    ScreenAngle angle;
    ScreenDist width;
    ScreenDist height;
};

template <typename T>
inline T remapDist(T dist, T fromMin, T fromMax, T toMin, T toMax)
{
    T fromRange = fromMax - fromMin;
    T toRange = toMax - toMin;
    return ((dist * toRange) / fromRange) + toMin;
}

template <typename T>
inline T remapValue(T value, T fromMin, T fromMax, T toMin, T toMax)
{
    value = std::clamp(value, fromMin, fromMax);
    value -= fromMin;
    return remapDist(value, fromMin, fromMax, toMin, toMax);
}

constexpr inline PhysicsDist inputDistToPhysicsDist(InputDist dist)
{
    return dist * 0.0254f;
}

constexpr inline PhysicsMass inputMassToPhysicsMass(InputMass mass)
{
    return mass * 0.453592f;
}

PhysicsAngle inputAngleToPhysicsAngle(InputAngle angle);
PhysicsPoint inputPointToPhysicsPoint(InputPoint point);
PhysicsPose inputPoseToPhysicsPose(InputPose pose);
PhysicsCircle inputCircleToPhysicsCircle(InputCircle circle);
PhysicsIdCircle inputCircleToPhysicsIdCircle(InputCircle circle,
                                             b2WorldId worldId,
                                             b2BodyDef &bodyDef,
                                             b2ShapeDef &shapeDef);
PhysicsRect inputRectToPhysicsRect(InputRect rect);
PhysicsIdRect inputRectToPhysicsIdRect(InputRect rect, b2WorldId worldId,
                                       b2BodyDef &bodyDef,
                                       b2ShapeDef &shapeDef);
PhysicsIdPolygon inputPolygonToPhysicsIdPolygon(InputPolygon polygon,
                                                b2WorldId worldId,
                                                b2BodyDef &bodyDef,
                                                b2ShapeDef &shapeDef);

ScreenDist physicsDistToScreenDist(PhysicsDist dist);
ScreenAngle physicsAngleToScreenAngle(PhysicsAngle angle);
ScreenPoint physicsPointToScreenPoint(PhysicsPoint point);
ScreenPose physicsPoseToScreenPose(PhysicsPose pose);
ScreenCircle physicsCircleToScreenCircle(PhysicsCircle circle);
ScreenCircle physicsIdCircleToScreenCircle(PhysicsIdCircle circle);
ScreenRect physicsRectToScreenRect(PhysicsRect rect);
ScreenRect physicsIdRectToScreenRect(PhysicsIdRect rect);

InputDist screenDistToInputDist(ScreenDist dist);
InputPoint screenPointToInputPoint(ScreenPoint point);

} // namespace bromine
