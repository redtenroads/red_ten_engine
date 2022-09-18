#pragma once
#include "utils.h"
#include "actor.h"
#include "inputManager.h"
#include "input.h"
#include <vector>

class ActorPawn : public Actor
{
public:
    EXPORT ActorPawn();
    EXPORT ~ActorPawn();

    template <class T, typename std::enable_if<std::is_base_of<Actor, T>::value>::type * = nullptr>
    EXPORT Input<Actor> *registerAxisCallback(void (T::*callback)(InputType, int, int, float))
    {
        return getInputManager()->registerAxisCallback(this, (void(Actor::*)(InputType, int, int, float))callback);
    }

    template <class T, typename std::enable_if<std::is_base_of<Actor, T>::value>::type * = nullptr>
    EXPORT Input<Actor> *registerButtonCallback(void (T::*callback)(InputType, int, int, bool))
    {
        return getInputManager()->registerButtonCallback(this, (void(Actor::*)(InputType, int, int, bool))callback);
    }

    EXPORT static void setInputManager(InputManager *inputManager);
    EXPORT static InputManager *getInputManager();

protected:
    static InputManager *inputManager;
};