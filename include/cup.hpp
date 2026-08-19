#pragma once
#include "circle.hpp"
#include "object.hpp"
#include "utils.hpp"
#include <memory>

namespace cobalt
{
class Cup : public Object
{
public:
    IdCircle circle;
    bool up;
    Cup(b2WorldId worldId, float x, float y, bool up,
        std::unique_ptr<Object> next = nullptr);
    void update(float dt) override;
    void draw(Renderer &renderer) const override;
};
} // namespace cobalt
