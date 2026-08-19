#pragma once
#include <box2d/box2d.h>

namespace cobalt
{
struct Rect
{
    b2Transform transform;
    float width;
    float height;
};
struct IdRect
{
    b2BodyId bodyId;
    float width;
    float height;
};
} // namespace cobalt
