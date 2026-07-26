#pragma once
#include "pin.hpp"
#include <vector>

namespace cobalt
{

class Field
{
  public:
    std::vector<Pin> pins;
    Field();
    void update(float dt);
};

}; // namespace cobalt
