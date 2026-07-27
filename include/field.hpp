#pragma once
#include "cup.hpp"
#include "pin.hpp"
#include <variant>
#include <vector>

namespace cobalt
{

class Field
{
  public:
    std::vector<std::variant<Pin, Cup>> objects;
    Field();
    void update(float dt);
};

}; // namespace cobalt
