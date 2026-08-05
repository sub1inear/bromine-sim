#pragma once

namespace cobalt
{
using Dist = float;

class Point
{
public:
    float x;
    float y;
};

class Pose
{
public:
    float x;
    float y;
    float theta;
};
inline Pose operator+(const Pose &lhs, float rhs)
{
    return {lhs.x + rhs, lhs.y + rhs, lhs.theta + rhs};
}
inline Pose operator+(const Pose &lhs, const Pose &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.theta + rhs.theta};
}
inline Pose &operator+=(Pose &lhs, float rhs)
{
    lhs = lhs + rhs;
    return lhs;
}
inline Pose &operator+=(Pose &lhs, const Pose &rhs)
{
    lhs = lhs + rhs;
    return lhs;
}
inline Pose operator-(const Pose &lhs, float rhs)
{
    return {lhs.x - rhs, lhs.y - rhs, lhs.theta - rhs};
}
inline Pose operator-(const Pose &lhs, const Pose &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.theta + rhs.theta};
}
inline Pose &operator-=(Pose &lhs, float rhs)
{
    lhs = lhs - rhs;
    return lhs;
}
inline Pose &operator-=(Pose &lhs, const Pose &rhs)
{
    lhs = lhs - rhs;
    return lhs;
}
inline Pose operator*(const Pose &lhs, float rhs)
{
    return {lhs.x * rhs, lhs.y * rhs, lhs.theta * rhs};
}
inline Pose &operator*=(Pose &lhs, float rhs)
{
    lhs = lhs * rhs;
    return lhs;
}
inline Pose operator/(const Pose &lhs, float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs, lhs.theta / rhs};
}
inline Pose &operator/=(Pose &lhs, float rhs)
{
    lhs = lhs / rhs;
    return lhs;
}

class Rect
{
public:
    float x;
    float y;
    Dist w;
    Dist h;
};
} // namespace cobalt
