#pragma once
#include "field.hpp"
#include <vector>

namespace cobalt
{
class Robot
{
  public:
    Field::Rectangle rect;
    float theta;
    Field::Pose velocity;

    Robot()
        : rect(0.0f, 0.0f, 10.0f, 10.0f), theta(0.0f),
          velocity(0.0f, 0.0f, 0.0f)
    {
    }
    void update(float dt);
};

} // namespace cobalt
