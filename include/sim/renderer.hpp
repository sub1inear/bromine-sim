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
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    void draw(const Sim &sim);

private:
    void drawHorizontalPin(const HorizontalPin &pin);
    void drawVerticalPin(const VerticalPin &pin);

    void drawDebug();
    void drawCup(const Cup &cup);
    void drawField(const Field &field);
    void drawRobot(const Robot &robot);
    void drawObject(const Object &object);
    void drawMotor(const Motor &motor);
    void drawWheel(const Wheel &wheel);
    void drawDevice(const Device &device);

    Texture2D fieldTexture;
    Texture2D pinsTexture;
    b2WorldId worldId;
    bool debug;
};
} // namespace bromine
