#include "pin.hpp"
#include "renderer.hpp"

namespace cobalt
{
void Pin::update(float dt) {}
void Pin::draw(Renderer &renderer) const { renderer.drawPin(*this); }
} // namespace cobalt
