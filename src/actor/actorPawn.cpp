// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "actor/actorPawn.h"

InputController *ActorPawn::inputController = nullptr;

ActorPawn::ActorPawn()
{
    registerName("ActorPawn");
}
ActorPawn::~ActorPawn()
{
    inputController->removeAllInputsByOwner(this);
}

void ActorPawn::setInputController(InputController *inputController)
{
    ActorPawn::inputController = inputController;
}

InputController *ActorPawn::getInputController()
{
    return ActorPawn::inputController;
}
