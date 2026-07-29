#pragma once

namespace cobalt
{
class Renderer;

class Object
{
  public:
    Object() = default;
    virtual ~Object() = default;
    virtual void update(float dt) = 0;
    virtual void draw(Renderer &renderer) const = 0; // Double-dispatch.
};
} // namespace cobalt
