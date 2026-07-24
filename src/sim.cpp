#include "sim.hpp"
#include "field.hpp"

namespace cobalt
{
void Sim::update(float dt)
{
    robot.update(dt);
    field.update(dt);
}
} // namespace cobalt
