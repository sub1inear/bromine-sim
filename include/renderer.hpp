#pragma once
#include <algorithm>
#include <raylib.h>

namespace cobalt
{
class Sim;
class Field;
class Robot;
class Object;
class Pin;
class Cup;

class Renderer
{
  public:
    friend class Pin;
    friend class Cup;

    Renderer();

    ~Renderer();

    void draw(const Sim &sim);

  private:
    void drawPin(const Pin &pin);
    void drawCup(const Cup &cup);
    void drawField(const Field &field);
    void drawRobot(const Robot &robot);
    void drawObject(const Object &object);

    Texture2D fieldTexture;
    Texture2D pinsTexture;
};
} // namespace cobalt
