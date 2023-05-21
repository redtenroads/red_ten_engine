// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/pawn.h"
#include "os/input.h"
#include "actor/actor.h"
#include <vector>

class InputController
{
public:
    InputController();

    EXPORT void provideInput(InputType type, int deviceIndex, int scancode, float value);
};
