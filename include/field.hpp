#pragma once
#include <array>

namespace cobalt
{

class Field
{
  public:
    using Dist = float;
    class Pose
    {
      public:
        float x;
        float y;
        float theta;
    };

    class Rectangle
    {
      public:
        float x;
        float y;
        Dist w;
        Dist h;
    };

    void update(float dt);
};

}; // namespace cobalt
