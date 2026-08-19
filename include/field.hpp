#pragma once
#include "object.hpp"
#include "rect.hpp"
#include <array>
#include <memory>
#include <vector>

namespace cobalt
{

class Field
{
public:
    std::vector<std::unique_ptr<Object>> objects;
    Field(b2WorldId worldId);
    void update(float dt);

private:
    std::array<IdRect, 4> walls;
};

}; // namespace cobalt
