#pragma once
#include "object.hpp"
#include <memory>
#include <vector>

namespace cobalt
{

class Field
{
public:
    std::vector<std::unique_ptr<Object>> objects;
    Field();
    void update(float dt);
};

}; // namespace cobalt
