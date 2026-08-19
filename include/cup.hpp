#pragma once
#include "object.hpp"
#include "units.hpp"
#include "utils.hpp"
#include <memory>

namespace cobalt
{
class Cup : public Object
{
public:
    PhysicsIdCircle circle;
    bool up;
    Cup(b2WorldId worldId, InputDist x, InputDist y, InputAngle angle, bool up,
        std::unique_ptr<Object> next = nullptr);
    void update(float dt) override;
    void draw(Renderer &renderer) const override;
};
} // namespace cobalt
