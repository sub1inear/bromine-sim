#pragma once

namespace cobalt
{
using Dist = float;

struct Point
{
    float x;
    float y;
};

struct Pose
{
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

struct Rect
{
    float x;
    float y;
    Dist w;
    Dist h;
};

struct RotatedRect
{
    float x;
    float y;
    float theta;
    Dist w;
    Dist h;
};

inline RotatedRect operator+(const RotatedRect &lhs, const Pose &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.theta + rhs.theta, lhs.w, lhs.h};
}
inline RotatedRect &operator+=(RotatedRect &lhs, const Pose &rhs)
{
    lhs = lhs + rhs;
    return lhs;
}
inline RotatedRect operator-(const RotatedRect &lhs, const Pose &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.theta + rhs.theta, lhs.w, lhs.h};
}
inline RotatedRect &operator-=(RotatedRect &lhs, const Pose &rhs)
{
    lhs = lhs - rhs;
    return lhs;
}

} // namespace cobalt
