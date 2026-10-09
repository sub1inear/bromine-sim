#pragma once
#include "v5_apitypes.h"
#include <array>
#include <atomic>
#include <cstdint>
#include <type_traits>

struct GLFWwindow;

namespace bromine
{
class Input
{
public:
    enum class Keys
    {
        None = 0,
        A = 1 << 0,
        B = 1 << 1,
        X = 1 << 2,
        Y = 1 << 3,
        L1 = 1 << 4,
        R1 = 1 << 5,
        R2 = 1 << 6,
        L2 = 1 << 7,
        Up = 1 << 8,
        Down = 1 << 9,
        Left = 1 << 10,
        Right = 1 << 11,
        Sel = 1 << 12,
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

    // NOTE: This needs to be <= 8 bytes to be atomic,
    // so we use uint8_t for the coordinates and force pressCount/releaseCount
    // to be an uint16_t. Technically this is invalid (should be uint32_t) but
    // nothing really can be done about it.
    struct Touch
    {
        uint8_t event;
        uint8_t y;
        uint16_t x;
        uint16_t pressCount;
        uint16_t releaseCount;
    };

    std::array<Controller, 2> controllers;

    std::atomic<Touch> touch;

    Input();
    void update(GLFWwindow &touchWindow);

private:
    void updateController();
    void updateKeyboard();
    void updateTouch(GLFWwindow &touchWindow);

    int lastTouchState;
};

} // namespace bromine
