#pragma once
#include "types.hpp"

namespace cobalt
{
class Cup
{
  public:
    Pose pose;
    Dist radius;
    bool up;
    Cup(float x, float y, bool up)
        : pose(x, y, 0.0f), radius(1.575f), up(up) {};
    void update(float dt);
};
} // namespace cobalt
