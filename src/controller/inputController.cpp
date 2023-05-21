// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "inputController.h"
#include <SDL.h>

InputController::InputController()
{
}

void InputController::provideInput(InputType type, int deviceIndex, int scancode, float value)
{
    auto inputs = Pawn::getInputs();
    for (auto it = inputs->begin(); it != inputs->end(); it++)
    {
        (*it)->provideInput(type, deviceIndex, scancode, value);
    }
}
