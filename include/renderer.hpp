#pragma once
#include <algorithm>
#include <raylib.h>

namespace cobalt
{
class Sim;
class Field;
class Robot;
class Object;
class HorizontalPin;
class VerticalPin;
class Cup;
class Motor;
class Device;
class DeviceContext;

class Renderer
{
public:
    friend class HorizontalPin;
    friend class VerticalPin;
    friend class Cup;
    friend class Motor;

    Renderer();

    ~Renderer();

    void draw(const Sim &sim);

private:
    void drawHorizontalPin(const HorizontalPin &pin);
    void drawVerticalPin(const VerticalPin &pin);

    void drawCup(const Cup &cup);
    void drawField(const Field &field);
    void drawRobot(const Robot &robot);
    void drawObject(const Object &object);
    void drawMotor(const Motor &motor, const DeviceContext &context);
    void drawDevice(const Device &device, const DeviceContext &context);

    Texture2D fieldTexture;
    Texture2D pinsTexture;
};
} // namespace cobalt
