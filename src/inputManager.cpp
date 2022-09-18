#include "inputManager.h"
#include "SDL.h"

InputManager::InputManager()
{
}

void InputManager::removeAllInputsByOwner(Actor *owner)
{
    auto it = inputs.begin();
    while (it != inputs.end())
        if ((*it)->getOwner() == owner)
        {
            delete (*it);
            it = inputs.erase(it);
        }
        else
            ++it;
}

void InputManager::provideInput(InputType type, int deviceIndex, int scancode, float value)
{
    for (auto it = inputs.begin(); it != inputs.end(); it++)
    {
        (*it)->provideInput(type, deviceIndex, scancode, value);
    }
}

Input<Actor> *InputManager::registerAxisCallback(Actor *actor, void (Actor::*callback)(InputType, int, int, float))
{
    Input<Actor> *input = new Input<Actor>(actor, callback, nullptr);
    inputs.push_back(input);
    return input;
}

Input<Actor> *InputManager::registerButtonCallback(Actor *actor, void (Actor::*callback)(InputType, int, int, bool))
{
    Input<Actor> *input = new Input<Actor>(actor, nullptr, callback);
    inputs.push_back(input);
    return input;
}
