#include "pin.hpp"
#include "renderer.hpp"

namespace cobalt
{
void HorizontalPin::update(float dt) {}
void HorizontalPin::draw(Renderer &renderer) const
{
    renderer.drawHorizontalPin(*this);
}

void VerticalPin::update(float dt) {}
void VerticalPin::draw(Renderer &renderer) const
{
    renderer.drawVerticalPin(*this);
}
} // namespace cobalt
