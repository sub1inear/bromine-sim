#pragma once
#include "raylib.h"
#include <GLFW/glfw3.h>

namespace vex_controller_shim
{
inline bool IsGamepadAvailable(int gamepad)
{
    return glfwJoystickPresent(gamepad == 0 ? GLFW_JOYSTICK_1 : gamepad);
}

inline const char *GetGamepadName(int gamepad)
{
    int jid = (gamepad == 0) ? GLFW_JOYSTICK_1 : gamepad;
    const char *name = glfwGetJoystickName(jid);
    return name ? name : "VEX V5 Controller";
}

inline float GetGamepadAxisMovement(int gamepad, int axis)
{
    int jid = (gamepad == 0) ? GLFW_JOYSTICK_1 : gamepad;
    int axesCount = 0;
    const float *axes = glfwGetJoystickAxes(jid, &axesCount);

    if (!axes || axis >= axesCount)
        return 0.0f;

    float val = axes[axis];
    return val;
}

inline bool IsGamepadButtonDown(int gamepad, int button)
{
    int jid = (gamepad == 0) ? GLFW_JOYSTICK_1 : gamepad;
    int buttonCount = 0;
    const unsigned char *buttons = glfwGetJoystickButtons(jid, &buttonCount);

    if (!buttons || button >= buttonCount)
        return false;
    return (buttons[button] == GLFW_PRESS);
}
} // namespace vex_controller_shim
