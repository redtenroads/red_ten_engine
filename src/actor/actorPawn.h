// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "actor/actor.h"
#include "controller/inputController.h"
#include "os/input.h"
#include <vector>

class ActorPawn : public Actor
{
public:
    EXPORT ActorPawn();
    EXPORT ~ActorPawn();

    template <class T, typename std::enable_if<std::is_base_of<Actor, T>::value>::type * = nullptr>
    EXPORT Input<Actor> *registerAxisCallback(void (T::*callback)(InputType, int, int, float))
    {
        return getInputController()->registerAxisCallback(this, (void(Actor::*)(InputType, int, int, float))callback);
    }

    template <class T, typename std::enable_if<std::is_base_of<Actor, T>::value>::type * = nullptr>
    EXPORT Input<Actor> *registerButtonCallback(void (T::*callback)(InputType, int, int, bool))
    {
        return getInputController()->registerButtonCallback(this, (void(Actor::*)(InputType, int, int, bool))callback);
    }

    EXPORT static void setInputController(InputController *inputController);
    EXPORT static InputController *getInputController();

protected:
    static InputController *inputController;
};