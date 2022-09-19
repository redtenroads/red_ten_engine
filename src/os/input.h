// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include <vector>

enum class InputType
{
    UNKNOWN = 0,
    ANY = 1,
    KEYBOARD = 2,
    MOUSE = 3,
    GAMEPAD_BUTTON = 4,
    GAMEPAD_AXIS = 5
};

enum class InputTypeMouse
{
    LEFT_BUTTON = 1,
    MIDDLE_BUTTON = 2,
    RIGHT_BUTTON = 3,
    BACK_BUTTON = 4,
    FRONT_BUTTON = 5,
    WHEEL_AXIS = 6,
    MOVE_HORIZONTAL = 7,
    MOVE_VERTICAL = 8
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

template <class T>
class Input
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

    float getAxisState() { return axisState; }

    bool getButtonState() { return axisState > 0.5f; }

    void provideInput(InputType type, int deviceIndex, int code, float value)
    {
        auto it = states.begin();
        while (it != states.end())
            if (
                (it->type == type || it->type == InputType::ANY) &&
                (it->deviceIndex == deviceIndex || it->deviceIndex == -1) &&
                (it->code == code || it->code == -1))
                it = states.erase(it);
            else
                ++it;

        if (type == InputType::GAMEPAD_AXIS && fabsf(value) < deadZone)
            value = 0.0f;

        for (auto it = bindings.begin(); it != bindings.end(); it++)
            if (
                (it->type == type || it->type == InputType::ANY) &&
                (it->deviceIndex == deviceIndex || it->deviceIndex == -1) &&
                (it->code == code || it->code == -1))
                states.push_back(BindingState({type, deviceIndex, code, value * it->modifier}));

        float output = 0.0f;
        if (states.size() > 0)
            for (auto it = states.begin(); it != states.end(); it++)
                if (fabsf(it->state) > fabsf(output))
                    output = it->state;
                

        if (output != axisState)
            setOutputState(type, deviceIndex, code, output);
    }

    EXPORT void setOutputState(InputType type, int deviceIndex, int code, float value)
    {
        axisState = value;
        if (callbackAsAxis)
            (((T *)owner)->*(callbackAsAxis))(type, deviceIndex, code, axisState);
        if (callbackAsButton)
            (((T *)owner)->*(callbackAsButton))(type, deviceIndex, code, axisState > 0.5f);
    }

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

    EXPORT void *getOwner()
    {
        return owner;
    }

    EXPORT void setDeadZone(float deadZone) { this->deadZone = deadZone; }
    EXPORT float getDeadZone() { return deadZone; }

protected:
    float axisState = 0.0f;
    float deadZone = 0.1f;
    std::vector<Binding> bindings;
    std::vector<BindingState> states;
    void *owner;
    void (T::*callbackAsAxis)(InputType, int, int, float);
    void (T::*callbackAsButton)(InputType, int, int, bool);
};
