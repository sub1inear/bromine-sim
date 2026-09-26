#include "input.hpp"
#include <raylib.h>

namespace bromine
{
constexpr int32_t axisMax = 127;
constexpr int32_t axisMin = -127;
constexpr float triggerThreshold = 0.5f;
constexpr size_t keyboardController = 0;

static int32_t axisToVex(int id, int axis)
{
    float value = GetGamepadAxisMovement(id, axis);
    return static_cast<int32_t>(value * axisMax);
}

static bool triggerToVex(int id, int button, int trigger)
{
    if (IsGamepadButtonDown(id, button))
    {
        return true;
    }
    float value = GetGamepadAxisMovement(id, trigger);
    return value > triggerThreshold;
}

Input::Input() : controllers{} {}

void Input::update()
{

    // NOTE: Y axes must be inverted to match PROS up is positive convention.
    for (int id = 0; id < controllers.size(); ++id)
    {
        if ((controllers[id].connected = IsGamepadAvailable(id)))
        {
            Keys keys = Keys::None;
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
            {
                keys |= Keys::A;
            }
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
            {
                keys |= Keys::B;
            }
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_RIGHT_FACE_UP))
            {
                keys |= Keys::X;
            }
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
            {
                keys |= Keys::Y;
            }
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_UP))
            {
                keys |= Keys::Up;
            }
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
            {
                keys |= Keys::Down;
            }
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
            {
                keys |= Keys::Left;
            }
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
            {
                keys |= Keys::Right;
            }
            if (triggerToVex(id, GAMEPAD_BUTTON_LEFT_TRIGGER_1,
                             GAMEPAD_AXIS_LEFT_TRIGGER))
            {
                keys |= Keys::L1;
            }
            if (triggerToVex(id, GAMEPAD_BUTTON_RIGHT_TRIGGER_1,
                             GAMEPAD_AXIS_RIGHT_TRIGGER))
            {
                keys |= Keys::R1;
            }
            if (triggerToVex(id, GAMEPAD_BUTTON_LEFT_TRIGGER_2,
                             GAMEPAD_AXIS_LEFT_TRIGGER))
            {
                keys |= Keys::L2;
            }
            if (triggerToVex(id, GAMEPAD_BUTTON_RIGHT_TRIGGER_2,
                             GAMEPAD_AXIS_RIGHT_TRIGGER))
            {
                keys |= Keys::R2;
            }
            if (IsGamepadButtonDown(id, GAMEPAD_BUTTON_MIDDLE_LEFT) ||
                IsGamepadButtonDown(id, GAMEPAD_BUTTON_MIDDLE) ||
                IsGamepadButtonDown(id, GAMEPAD_BUTTON_MIDDLE_RIGHT))
            {
                keys |= Keys::Sel;
            }
            controllers[id].keys = keys;

            controllers[id].leftAxisY = -axisToVex(id, GAMEPAD_AXIS_LEFT_Y);
            controllers[id].rightAxisX = axisToVex(id, GAMEPAD_AXIS_RIGHT_X);
            controllers[id].leftAxisX = axisToVex(id, GAMEPAD_AXIS_LEFT_X);
            controllers[id].rightAxisY = -axisToVex(id, GAMEPAD_AXIS_RIGHT_Y);
        }
    }

    controllers[keyboardController].connected = true;
    Keys keys = Keys::None;

    if (IsKeyDown(KEY_W))
    {
        controllers[keyboardController].leftAxisY = axisMax;
    }
    if (IsKeyDown(KEY_S))
    {
        controllers[keyboardController].leftAxisY = axisMin;
    }
    if (IsKeyDown(KEY_A))
    {
        controllers[keyboardController].leftAxisX = axisMin;
    }
    if (IsKeyDown(KEY_D))
    {
        controllers[keyboardController].leftAxisX = axisMax;
    }
    if (IsKeyDown(KEY_I))
    {
        controllers[keyboardController].rightAxisY = axisMax;
    }
    if (IsKeyDown(KEY_K))
    {
        controllers[keyboardController].rightAxisY = axisMin;
    }
    if (IsKeyDown(KEY_J))
    {
        controllers[keyboardController].rightAxisX = axisMin;
    }
    if (IsKeyDown(KEY_L))
    {
        controllers[keyboardController].rightAxisX = axisMax;
    }
    if (IsKeyDown(KEY_Q))
    {
        keys |= Keys::L1;
    }
    if (IsKeyDown(KEY_E))
    {
        keys |= Keys::L2;
    }
    if (IsKeyDown(KEY_U))
    {
        keys |= Keys::R1;
    }
    if (IsKeyDown(KEY_O))
    {
        keys |= Keys::R2;
    }
    if (IsKeyDown(KEY_Z))
    {
        keys |= Keys::A;
    }
    if (IsKeyDown(KEY_X))
    {
        keys |= Keys::B;
    }
    if (IsKeyDown(KEY_C))
    {
        keys |= Keys::X;
    }
    if (IsKeyDown(KEY_V))
    {
        keys |= Keys::Y;
    }
    if (IsKeyDown(KEY_P))
    {
        keys |= Keys::Sel;
    }

    controllers[keyboardController].keys = keys;

    if (IsKeyPressed(KEY_GRAVE))
    {
        debug = !debug;
    }
}

} // namespace bromine
