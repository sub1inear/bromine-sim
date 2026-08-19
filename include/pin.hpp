#pragma once
#include "circle.hpp"
#include "object.hpp"
#include "rect.hpp"

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
    IdRect rect;
    HorizontalPin(b2WorldId worldId, float x, float y, float theta, Type type);
    void update(float dt) override final;
    void draw(Renderer &renderer) const override final;
};

class VerticalPin : public Pin
{
public:
    IdCircle circle;
    bool isUp;
    VerticalPin(b2WorldId worldId, float x, float y, bool isUp, Type type);
    void update(float dt) override final;
    void draw(Renderer &renderer) const override final;
};

} // namespace cobalt
