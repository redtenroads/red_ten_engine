#include "input.h"

void InputBase::provideInput(InputType type, int deviceIndex, int code, float value)
{
    if (type == InputType::KEYBOARD)
        processKeyboard(code, value);

    if (type == InputType::MOUSE)
        processMouse((InputTypeMouse)deviceIndex, code, value);

    if (type == InputType::GAMEPAD_AXIS)
        processGamepadAxis(deviceIndex, code, value);

    if (type == InputType::GAMEPAD_BUTTON)
        processGamepadButton(deviceIndex, code, value);
}

void InputBase::processKeyboard(int code, float value)
{
    if (states.size() > 0)
    {
        auto it = states.begin();
        while (it != states.end())
            if (it->type == InputType::KEYBOARD && (it->code == -1 || it->code == code))
                it = states.erase(it);
            else
                ++it;
    }

    if (bindings.size() > 0)
    {
        for (auto it = bindings.begin(); it != bindings.end(); it++)
        {
            if (it->type == InputType::KEYBOARD && (it->code == -1 || it->code == code))
                states.push_back(BindingState({InputType::KEYBOARD, -1, code, value * it->modifier}));
        }
    }

    updateRelativeOutput();
}

void InputBase::processMouse(InputTypeMouse inputTypeMouse, int code, float value)
{
    if (inputTypeMouse == InputTypeMouse::MOVEMENT)
    {
        // find if we subscribed to this
        if (bindings.size() > 0)
        {
            for (auto it = bindings.begin(); it != bindings.end(); it++)
            {
                if (it->type == InputType::MOUSE && (it->code == (int)InputTypeMouseMove::MOVE || it->code == code))
                    updateAbsoluteOutput(InputType::MOUSE, (int)InputTypeMouse::MOVEMENT, code, value);
            }
        }
    }
    else
    {
        if (states.size() > 0)
        {
            auto it = states.begin();
            while (it != states.end())
                if (it->type == InputType::MOUSE && (it->code == -1 || it->code == code))
                    it = states.erase(it);
                else
                    ++it;
        }

        if (bindings.size() > 0)
        {
            for (auto it = bindings.begin(); it != bindings.end(); it++)
            {
                if (it->type == InputType::MOUSE && (it->code == -1 || it->code == code))
                    states.push_back(BindingState({InputType::MOUSE, -1, code, value * it->modifier}));
            }
        }

        updateRelativeOutput();
    }
}

void InputBase::processGamepadAxis(int deviceId, int code, float value)
{
    value = fabsf(value) < deadZone ? 0.0f : value;

    if (states.size() > 0)
    {
        auto it = states.begin();
        while (it != states.end())
            if (it->type == InputType::GAMEPAD_AXIS && (it->code == -1 || it->code == code))
                it = states.erase(it);
            else
                ++it;
    }

    if (bindings.size() > 0)
    {
        for (auto it = bindings.begin(); it != bindings.end(); it++)
        {
            if (it->type == InputType::GAMEPAD_AXIS && (it->code == -1 || it->code == code))
                states.push_back(BindingState({InputType::GAMEPAD_AXIS, -1, code, value * it->modifier}));
        }
    }

    updateRelativeOutput();
}

void InputBase::processGamepadButton(int deviceId, int code, float value)
{
    if (states.size() > 0)
    {
        auto it = states.begin();
        while (it != states.end())
            if (it->type == InputType::GAMEPAD_BUTTON && (it->code == -1 || it->code == code))
                it = states.erase(it);
            else
                ++it;
    }

    if (bindings.size() > 0)
    {
        for (auto it = bindings.begin(); it != bindings.end(); it++)
        {
            if (it->type == InputType::GAMEPAD_BUTTON && (it->code == -1 || it->code == code))
                states.push_back(BindingState({InputType::GAMEPAD_BUTTON, -1, code, value * it->modifier}));
        }
    }

    updateRelativeOutput();
}

void InputBase::updateRelativeOutput()
{
    float output = 0.0f;
    InputType type = InputType::UNKNOWN;
    int deviceIndex = -1;
    int code = -1;

    if (states.size() > 0)
        for (auto it = states.begin(); it != states.end(); it++)
            if (fabsf(it->state) > fabsf(output))
            {
                type = it->type;
                output = it->state;
                deviceIndex = it->deviceIndex;
                code = it->code;
            }

    if (axisState != output)
    {
        axisState = output;
        setOutputState(type, deviceIndex, code, output);
    }
}

void InputBase::updateAbsoluteOutput(InputType type, int deviceId, int code, float output)
{
    setOutputState(type, deviceId, code, output);
}

void InputBase::addKeyboardBinding(float modifier)
{
    bindings.push_back(Binding({InputType::KEYBOARD, -1, -1, modifier}));
}

void InputBase::addKeyboardBinding(int code, float modifier)
{
    bindings.push_back(Binding({InputType::KEYBOARD, -1, code, modifier}));
}

void InputBase::addKeyboardBinding(KeyboardCodes code, float modifier)
{
    bindings.push_back(Binding({InputType::KEYBOARD, -1, (int)code, modifier}));
}

void InputBase::addMouseButtonBinding(float modifier)
{
    bindings.push_back(Binding({InputType::MOUSE, (int)InputTypeMouse::ALL, -1, modifier}));
}

void InputBase::addMouseButtonBinding(InputTypeMouse button, float modifier)
{
    bindings.push_back(Binding({InputType::MOUSE, (int)button, -1, modifier}));
}

void InputBase::addMouseMoveBinding(float modifier)
{
    bindings.push_back(Binding({InputType::MOUSE, (int)InputTypeMouse::MOVEMENT, (int)InputTypeMouseMove::MOVE, modifier}));
}

void InputBase::addMouseMoveBinding(InputTypeMouseMove direction, float modifier)
{
    bindings.push_back(Binding({InputType::MOUSE, (int)InputTypeMouse::MOVEMENT, (int)direction, modifier}));
}

void InputBase::addGamepadButtonBinding(int code, float modifier)
{
    bindings.push_back(Binding({InputType::GAMEPAD_BUTTON, -1, code, modifier}));
}

void InputBase::addGamepadAxisBinding(int code, float modifier)
{
    bindings.push_back(Binding({InputType::GAMEPAD_AXIS, -1, code, modifier}));
}

void InputBase::addGamepadButtonBinding(int code, int deviceIndex, float modifier)
{
    bindings.push_back(Binding({InputType::GAMEPAD_BUTTON, deviceIndex, code, modifier}));
}

void InputBase::addGamepadAxisBinding(int code, int deviceIndex, float modifier)
{
    bindings.push_back(Binding({InputType::GAMEPAD_AXIS, deviceIndex, code, modifier}));
}