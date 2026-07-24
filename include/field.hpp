#pragma once
#include <array>

namespace cobalt
{

class Field
{
  public:
    using Dist = float;
    class Pos
    {
      public:
        float x;
        float y;
    };
    class Rect
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
