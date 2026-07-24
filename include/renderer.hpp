#pragma once
#include "field.hpp"
#include <algorithm>
#include <raylib.h>

namespace cobalt
{
class Sim;
class Field;
class Robot;

class Renderer
{
  public:
    Renderer();

    ~Renderer();

    void draw(const Sim &sim);

  private:
    void drawField(const Field &field);
    void drawRobot(const Robot &robot);

    Texture2D fieldTexture;
};
} // namespace cobalt
