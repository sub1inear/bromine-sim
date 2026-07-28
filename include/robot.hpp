#pragma once
#include "types.hpp"
#include <vector>

namespace cobalt
{
class Robot
{
  public:
    Rect rect;
    float theta; // In radians.
    Pose velocity;
    float mass; // In lbs.

    Robot()
        : rect(0.0f, 0.0f, 10.0f, 10.0f), theta(0.0f),
          velocity(0.0f, 0.0f, 0.0f), mass(16.0f)
    {
    }
    void update(float dt);

  private:
    bool collidesWithWall();
};

} // namespace cobalt
