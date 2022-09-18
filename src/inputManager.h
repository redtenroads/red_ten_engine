#pragma once
#include "utils.h"
#include "input.h"
#include "actor.h"
#include <vector>

class InputManager
{
public:
    InputManager();

    EXPORT void removeAllInputsByOwner(Actor *owner);
    EXPORT void provideInput(InputType type, int deviceIndex, int scancode, float value);

    EXPORT Input<Actor> *registerAxisCallback(Actor *actor, void (Actor::*callback)(InputType, int, int, float));
    EXPORT Input<Actor> *registerButtonCallback(Actor *actor, void (Actor::*callback)(InputType, int, int, bool));

protected:
    std::vector<Input<Actor> *> inputs;
};
