#pragma once
#include "object.hpp"
#include "types.hpp"

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
    RotatedRect rect;
    HorizontalPin(float x, float y, float theta, Type type)
        : rect(x, y, theta, 3.2f, 6.5f), Pin(type)
    {
    }
    void update(float dt) override final;
    void draw(Renderer &renderer) const override final;
};

class VerticalPin : public Pin
{
public:
    Point point;
    Dist radius;
    bool up;
    VerticalPin(float x, float y, bool up, Type type)
        : point(x, y), radius(1.0f), up(up), Pin(type)
    {
    }
    void update(float dt) override final;
    void draw(Renderer &renderer) const override final;
};

} // namespace cobalt
