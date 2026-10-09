#include "pin.hpp"
#include "categories.hpp"
#include "constants.hpp"
#include "renderer.hpp"
#include "utils.hpp"

namespace bromine
{
HorizontalPin::HorizontalPin(b2WorldId worldId, InputDist x, InputDist y,
                             InputAngle angle, Type type)
    : Pin(type)
{
    InputRect inputRect = {x, y, angle, 3.2f, 6.5f, 0.158733f};

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.linearDamping = 0.9f;
    bodyDef.angularDamping = 0.9f;
    bodyDef.type = b2_dynamicBody;
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.filter.categoryBits = std::to_underlying(Categories::Object);

    rect = inputRectToPhysicsIdRect(inputRect, worldId, bodyDef, shapeDef);
}

void HorizontalPin::preUpdate() {}

void HorizontalPin::postUpdate() {}

void HorizontalPin::draw(Renderer &renderer) const
{
    renderer.drawHorizontalPin(*this);
}

VerticalPin::VerticalPin(b2WorldId worldId, InputDist x, InputDist y,
                         InputAngle angle, bool isUp, Type type)
    : Pin(type), isUp(isUp)
{
    InputCircle inputCircle = {x, y, angle, 1.575f};
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    circle =
        inputCircleToPhysicsIdCircle(inputCircle, worldId, bodyDef, shapeDef);
}

void VerticalPin::preUpdate() {}

void VerticalPin::postUpdate() {}

void VerticalPin::draw(Renderer &renderer) const
{
    renderer.drawVerticalPin(*this);
}
} // namespace bromine
