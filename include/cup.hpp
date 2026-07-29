#pragma once
#include "object.hpp"
#include "types.hpp"

namespace cobalt
{
class Cup : public Object
{
  public:
    Pose pose;
    Dist radius;
    bool up;
    Cup(float x, float y, bool up)
        : pose(x, y, 0.0f), radius(1.575f), up(up) {};
    void update(float dt) override;
    void draw(Renderer &renderer) const override;
};
} // namespace cobalt
