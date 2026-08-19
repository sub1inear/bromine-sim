#include "pin.hpp"
#include "renderer.hpp"
#include "utils.hpp"

namespace cobalt
{
HorizontalPin::HorizontalPin(b2WorldId worldId, float x, float y, float theta,
                             Type type)
    : Pin(type)
{
    // TODO: Create a polygon.
    rect.width = inchesToMeters(1.6);
    rect.height = inchesToMeters(6.5);
    // TODO: Apply mass.

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {inchesToMeters(x), inchesToMeters(y)};
    bodyDef.rotation = b2MakeRot(cwToCcw(theta));
    rect.bodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon boxDef = b2MakeBox(rect.width / 2.0f, rect.height / 2.0f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    b2ShapeId shapeId = b2CreatePolygonShape(rect.bodyId, &shapeDef, &boxDef);
}

void HorizontalPin::update(float dt) {}
void HorizontalPin::draw(Renderer &renderer) const
{
    renderer.drawHorizontalPin(*this);
}

VerticalPin::VerticalPin(b2WorldId worldId, float x, float y, bool isUp,
                         Type type)
    : Pin(type), isUp(isUp)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {inchesToMeters(x), inchesToMeters(y)};
    circle.bodyId = b2CreateBody(worldId, &bodyDef);
    circle.radius = inchesToMeters(1.575f);

    b2Circle circleDef = {{0.0f, 0.0f}, circle.radius};
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    b2ShapeId shapeId =
        b2CreateCircleShape(circle.bodyId, &shapeDef, &circleDef);
}

void VerticalPin::update(float dt) {}
void VerticalPin::draw(Renderer &renderer) const
{
    renderer.drawVerticalPin(*this);
}
} // namespace cobalt
