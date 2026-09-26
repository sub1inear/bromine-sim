#pragma once
#include <array>
#include <atomic>
#include <type_traits>

namespace bromine
{
class Input
{
public:
    enum class Keys
    {
        None = 1 << 0,
        A = 1 << 1,
        B = 1 << 2,
        X = 1 << 3,
        Y = 1 << 4,
        L1 = 1 << 5,
        R1 = 1 << 6,
        R2 = 1 << 7,
        L2 = 1 << 8,
        Up = 1 << 9,
        Down = 1 << 10,
        Left = 1 << 11,
        Right = 1 << 12,
        Sel = 1 << 13,
    };

    friend constexpr Keys operator|(Keys a, Keys b)
    {
        using T = std::underlying_type_t<Keys>;
        return static_cast<Keys>(static_cast<T>(a) | static_cast<T>(b));
    }

    friend constexpr Keys operator&(Keys a, Keys b)
    {
        using T = std::underlying_type_t<Keys>;
        return static_cast<Keys>(static_cast<T>(a) & static_cast<T>(b));
    }

    friend constexpr Keys &operator|=(Keys &a, Keys b)
    {
        a = a | b;
        return a;
    }

    friend constexpr Keys &operator&=(Keys &a, Keys b)
    {
        a = a & b;
        return a;
    }

    // NOTE: This code could be optimized to eliminate the connected variable
    // for the first controller, but this is not worth it in terms of
    // complexity.

    struct Controller
    {
        std::atomic<Keys> keys;
        std::atomic<int32_t> leftAxisX;
        std::atomic<int32_t> leftAxisY;
        std::atomic<int32_t> rightAxisX;
        std::atomic<int32_t> rightAxisY;
        std::atomic<bool> connected;
    };

    std::array<Controller, 2> controllers;

    bool debug;

    Input();
    void update();
};

} // namespace bromine
