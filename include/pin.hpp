#pragma once
#include "object.hpp"
#include "types.hpp"

namespace cobalt
{
class Pin : public Object
{
  public:
    // Note: Corresponds to spritesheet order for speed.
    enum class Kind
    {
        YellowYellow,
        RedYellow,
        BlueYellow,
        RedBlue,
    };
    Kind kind;
    Pin(Kind kind) : kind(kind) {};
    void update(float dt) override = 0;
    void draw(Renderer &renderer) const override = 0;
};

class HorizontalPin : public Pin
{
  public:
    Rect rect;
    float theta; // In radians.
    HorizontalPin(float x, float y, float theta, Kind kind)
        : rect(x, y, 3.2f, 6.5f), theta(theta), Pin(kind)
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
    VerticalPin(float x, float y, bool up, Kind kind)
        : point(x, y), radius(1.0f), up(up), Pin(kind)
    {
    }
    void update(float dt) override final;
    void draw(Renderer &renderer) const override final;
};

} // namespace cobalt
