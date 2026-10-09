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
class Imu;
class Distance;
class Device;
class State;

class Renderer
{
public:
    friend class HorizontalPin;
    friend class VerticalPin;
    friend class Cup;
    friend class Motor;
    friend class Wheel;
    friend class Imu;
    friend class Distance;

    Renderer(b2WorldId worldId);
    ~Renderer();
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    void draw(const Sim &sim);

private:
    void drawHorizontalPin(const HorizontalPin &pin);
    void drawVerticalPin(const VerticalPin &pin);

    void drawDebug(const Sim &sim);
    void drawState(const State &state);
    void drawStateTimer(const State &state);
    void drawStateType(const State &state);
    void drawStatePhase(const State &state);
    void drawCup(const Cup &cup);
    void drawField(const Field &field);
    void drawRobot(const Robot &robot);
    void drawObject(const Object &object);
    void drawMotor(const Motor &motor);
    void drawWheel(const Wheel &wheel);
    void drawImu(const Imu &imu);
    void drawDistance(const Distance &distance);
    void drawDevice(const Device &device);

    Texture2D fieldTexture;
    Texture2D pinsTexture;
    b2WorldId worldId;
    bool debug;
};
} // namespace bromine
