#pragma once
#include "types.hpp"

namespace cobalt
{
class Pin
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
    Rect rect;
    float theta; // In radians.
    Kind kind;
    Pin(float x, float y, float theta, Kind kind)
        : rect(x, y, 3.2f, 6.5f), theta(theta), kind(kind)
    {
    }
    void update(float dt);
};
} // namespace cobalt
