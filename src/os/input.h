// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/keyboard.h"
#include <vector>

enum class InputType
{
    UNKNOWN = 0,
    KEYBOARD = 1,
    MOUSE = 2,
    GAMEPAD_BUTTON = 3,
    GAMEPAD_AXIS = 4
};

enum class InputTypeMouse
{
    ALL = 0,
    LEFT_BUTTON = 1,
    MIDDLE_BUTTON = 2,
    RIGHT_BUTTON = 3,
    BACK_BUTTON = 4,
    FRONT_BUTTON = 5,
    MOVEMENT = 6
};

enum class InputTypeMouseMove
{
    MOVE = 0,
    MOVE_HORIZONTAL = 1,
    MOVE_VERTICAL = 2,
    MOVE_WHEEL = 3
};

struct Binding
{
    InputType type;
    int deviceIndex;
    int code;
    float modifier;
};

struct BindingState
{
    InputType type;
    int deviceIndex;
    int code;
    float state;
};

class InputBase
{
public:
    EXPORT virtual ~InputBase(){};

    EXPORT void provideInput(InputType type, int deviceIndex, int code, float value);

    EXPORT float getAxisState() { return axisState; }
    EXPORT bool getButtonState() { return axisState > 0.5f; }

    EXPORT void processKeyboard(int code, float value);
    EXPORT void processMouse(InputTypeMouse inputTypeMouse, int code, float output);
    EXPORT void processGamepadAxis(int deviceId, int code, float value);
    EXPORT void processGamepadButton(int deviceId, int code, float value);

    EXPORT void updateRelativeOutput();
    EXPORT void updateAbsoluteOutput(InputType type, int deviceId, int code, float output);

    EXPORT void addKeyboardBinding(float modifier);
    EXPORT void addKeyboardBinding(int code, float modifier);
    EXPORT void addKeyboardBinding(KeyboardCodes code, float modifier);

    EXPORT void addMouseButtonBinding(float modifier);
    EXPORT void addMouseButtonBinding(InputTypeMouse button, float modifier);

    EXPORT void addMouseMoveBinding(float modifier);
    EXPORT void addMouseMoveBinding(InputTypeMouseMove direction, float modifier);

    EXPORT void addGamepadButtonBinding(int code, float modifier);
    EXPORT void addGamepadAxisBinding(int code, float modifier);
    EXPORT void addGamepadButtonBinding(int code, int deviceIndex, float modifier);
    EXPORT void addGamepadAxisBinding(int code, int deviceIndex, float modifier);

    EXPORT void *getOwner() { return owner; }

    EXPORT void setDeadZone(float deadZone) { this->deadZone = deadZone; }
    EXPORT float getDeadZone() { return deadZone; }

    EXPORT virtual void setOutputState(InputType type, int deviceIndex, int code, float value){};

protected:
    float axisState = 0.0f;
    float deadZone = 0.1f;
    std::vector<Binding> bindings;
    std::vector<BindingState> states;
    void *owner = nullptr;
};

template <class T>
class Input : public InputBase
{
public:
    EXPORT Input(
        void *owner,
        void (T::*callbackAsAxis)(InputType, int, int, float),
        void (T::*callbackAsButton)(InputType, int, int, bool))
    {
        this->owner = owner;
        this->callbackAsAxis = callbackAsAxis;
        this->callbackAsButton = callbackAsButton;
    }

    EXPORT void setOutputState(InputType type, int deviceIndex, int code, float value)
    {
        axisState = value;
        if (callbackAsAxis)
            (((T *)owner)->*(callbackAsAxis))(type, deviceIndex, code, axisState);
        if (callbackAsButton)
            (((T *)owner)->*(callbackAsButton))(type, deviceIndex, code, axisState > 0.5f);
    }

    /*
        EXPORT void addInputBinding(float modifier)
        {
            if (!doesMatch(InputType::ANY, -1, -1))
                bindings.push_back(Binding({InputType::ANY, -1, -1, modifier}));
        }

        EXPORT void addInputBinding(InputType type, float modifier)
        {
            if (!doesMatch(type, -1, -1))
                bindings.push_back(Binding({type, -1, -1, modifier}));
        }

        EXPORT void addInputBinding(InputType type, int code, float modifier)
        {
            addInputBinding(type, -1, code, modifier);
        }

        EXPORT void addInputBinding(InputType type, int index, int code, float modifier)
        {
            if (!doesMatch(type, index, code))
                bindings.push_back(Binding({type, index, code, modifier}));
        }

        EXPORT void removeInputBinding(InputType type, int index, int code)
        {
            auto it = states.begin();
            while (it != states.end())
                if (it->type == type && it->code == code && it->index == index)
                    it = states.erase(it);
                else
                    ++it;
        }

        EXPORT void removeInputBinding(InputType type, int code)
        {
            removeInputBinding(type, -1, code);
        }

        EXPORT bool doesMatch(InputType type, int index, int code)
        {
            for (auto it = bindings.begin(); it != bindings.end(); it++)
                if (it->type == type && it->code == code && it->deviceIndex == index)
                    return true;
            return false;
        }

        EXPORT bool doesMatch(InputType type, int code)
        {
            return doesMatch(type, -1, code);
        }
        */
protected:
    void (T::*callbackAsAxis)(InputType, int, int, float);
    void (T::*callbackAsButton)(InputType, int, int, bool);
};
