#pragma once

namespace cobalt
{
using Dist = float;
class Pose
{
  public:
    float x;
    float y;
    float theta;
};

class Rect
{
  public:
    float x;
    float y;
    Dist w;
    Dist h;
};
} // namespace cobalt
