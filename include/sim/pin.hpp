#pragma once
#include "object.hpp"
#include "units.hpp"

namespace bromine
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
    void preUpdate() override = 0;
    void postUpdate() override = 0;
    void draw(Renderer &renderer) const override = 0;
};

class HorizontalPin : public Pin
{
public:
    PhysicsIdRect rect;
    HorizontalPin(b2WorldId worldId, InputDist x, InputDist y, InputAngle angle,
                  Type type);
    void preUpdate() override final;
    void postUpdate() override final;
    void draw(Renderer &renderer) const override final;
};

class VerticalPin : public Pin
{
public:
    PhysicsIdCircle circle;
    bool isUp;
    VerticalPin(b2WorldId worldId, InputDist x, InputDist y, InputAngle angle,
                bool isUp, Type type);
    void preUpdate() override final;
    void postUpdate() override final;
    void draw(Renderer &renderer) const override final;
};

} // namespace bromine
