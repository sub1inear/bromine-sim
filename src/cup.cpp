#include "cup.hpp"
#include "renderer.hpp"

namespace cobalt
{
void Cup::update(float dt) {}
void Cup::draw(Renderer &renderer) const { renderer.drawCup(*this); }

} // namespace cobalt
