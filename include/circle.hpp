#pragma once
#include <box2d/box2d.h>

namespace cobalt
{

struct Circle
{
    b2Transform transform;
    float radius;
};
struct IdCircle
{
    b2BodyId bodyId;
    float radius;
};

} // namespace cobalt
