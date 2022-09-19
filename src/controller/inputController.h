// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "os/input.h"
#include "actor/actor.h"
#include <vector>

class InputController
{
public:
    InputController();

    EXPORT void removeAllInputsByOwner(Actor *owner);
    EXPORT void provideInput(InputType type, int deviceIndex, int scancode, float value);

    EXPORT Input<Actor> *registerAxisCallback(Actor *actor, void (Actor::*callback)(InputType, int, int, float));
    EXPORT Input<Actor> *registerButtonCallback(Actor *actor, void (Actor::*callback)(InputType, int, int, bool));

protected:
    std::vector<Input<Actor> *> inputs;
};
