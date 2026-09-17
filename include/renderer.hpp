#pragma once
#include "units.hpp"
#include <algorithm>
#include <box2d/box2d.h>
#include <raylib.h>

namespace bromine
{
class Sim;
class Field;
class Robot;
class Object;
class HorizontalPin;
class VerticalPin;
class Cup;
class Motor;
class Wheel;
class Device;
class DeviceContext;

class Renderer
{
public:
    friend class HorizontalPin;
    friend class VerticalPin;
    friend class Cup;
    friend class Motor;
    friend class Wheel;

    Renderer(b2WorldId worldId);

    ~Renderer();

    void draw(const Sim &sim);

private:
    void drawHorizontalPin(const HorizontalPin &pin);
    void drawVerticalPin(const VerticalPin &pin);

    void drawDebug();
    void drawCup(const Cup &cup);
    void drawField(const Field &field);
    void drawRobot(const Robot &robot);
    void drawObject(const Object &object);
    void drawMotor(const Motor &motor, PhysicsPose parentPose);
    void drawWheel(const Wheel &wheel, PhysicsPose parentPose);
    void drawDevice(const Device &device, PhysicsPose parentPose);

    Texture2D fieldTexture;
    Texture2D pinsTexture;
    b2WorldId worldId;
    bool debug;
};
} // namespace bromine
