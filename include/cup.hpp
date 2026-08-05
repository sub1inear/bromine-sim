#pragma once
#include "object.hpp"
#include "types.hpp"
#include <memory>

namespace cobalt
{
class Cup : public Object
{
public:
    Point point;
    Dist radius;
    bool up;
    Cup(float x, float y, bool up, std::unique_ptr<Object> next = nullptr)
        : point(x, y), radius(1.575f), up(up), Object(std::move(next)) {};
    void update(float dt) override;
    void draw(Renderer &renderer) const override;
};
} // namespace cobalt
