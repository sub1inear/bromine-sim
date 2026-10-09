#pragma once
#include "object.hpp"
#include "units.hpp"
#include <array>
#include <memory>
#include <vector>

namespace bromine
{

class Field
{
public:
    std::vector<std::unique_ptr<Object>> objects;
    Field(b2WorldId worldId);
    void preUpdate();
    void postUpdate();

private:
    std::array<PhysicsIdRect, 4> walls;
};

}; // namespace bromine
