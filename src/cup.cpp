#include "cup.hpp"
#include "renderer.hpp"
#include "utils.hpp"

namespace bromine
{
Cup::Cup(b2WorldId worldId, InputDist x, InputDist y, InputAngle angle, bool up,
         std::unique_ptr<Object> next)
    : Object(std::move(next)), up(up)
{
    InputCircle inputCircle = {x, y, angle, 1.575f, 0.169756f};

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.linearDamping = 0.9f;
    bodyDef.angularDamping = 0.9f;
    bodyDef.type = b2_dynamicBody;
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    circle =
        inputCircleToPhysicsIdCircle(inputCircle, worldId, bodyDef, shapeDef);
}

void Cup::update(float dt) {}

void Cup::draw(Renderer &renderer) const { renderer.drawCup(*this); }

} // namespace bromine
