#pragma once
#include <box2d/box2d.h>
#include <memory>
#include <vector>

namespace bromine
{
class Renderer;

class Object
{
public:
    std::unique_ptr<Object> next;
    Object(std::unique_ptr<Object> next = nullptr) : next(std::move(next)) {};
    virtual ~Object() = default;
    virtual void preUpdate() = 0;
    virtual void postUpdate() = 0;
    virtual void draw(Renderer &renderer) const = 0; // Double-dispatch.
};
} // namespace bromine
