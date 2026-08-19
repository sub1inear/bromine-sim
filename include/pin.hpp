#pragma once
#include "object.hpp"
#include "units.hpp"

namespace cobalt
{
class Pin : public Object
{
public:
    // Note: Corresponds to spritesheet order for speed.
    enum class Type
    {
        YellowYellow,
        RedYellow,
        BlueYellow,
        RedBlue,
    };
    Type type;
    Pin(Type type) : type(type) {};
    void update(float dt) override = 0;
    void draw(Renderer &renderer) const override = 0;
};

class HorizontalPin : public Pin
{
public:
    PhysicsIdRect rect;
    HorizontalPin(b2WorldId worldId, InputDist x, InputDist y, InputAngle angle,
                  Type type);
    void update(float dt) override final;
    void draw(Renderer &renderer) const override final;
};

class VerticalPin : public Pin
{
public:
    PhysicsIdCircle circle;
    bool isUp;
    VerticalPin(b2WorldId worldId, InputDist x, InputDist y, InputAngle angle,
                bool isUp, Type type);
    void update(float dt) override final;
    void draw(Renderer &renderer) const override final;
};

} // namespace cobalt
