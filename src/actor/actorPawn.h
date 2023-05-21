// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/pawn.h"
#include "actor/actor.h"

class ActorPawn : public Actor, public Pawn
{
public:
    EXPORT ActorPawn();
};