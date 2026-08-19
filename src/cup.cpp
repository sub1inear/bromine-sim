#include "cup.hpp"
#include "renderer.hpp"
#include "utils.hpp"

namespace cobalt
{
Cup::Cup(b2WorldId worldId, float x, float y, bool up,
         std::unique_ptr<Object> next)
    : Object(std::move(next)), up(up)
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

void Cup::update(float dt) {}
void Cup::draw(Renderer &renderer) const { renderer.drawCup(*this); }

} // namespace cobalt
