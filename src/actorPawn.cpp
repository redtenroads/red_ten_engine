#include "actorPawn.h"

InputManager *ActorPawn::inputManager = nullptr;

ActorPawn::ActorPawn()
{
    registerName("ActorPawn");
}
ActorPawn::~ActorPawn()
{
    inputManager->removeAllInputsByOwner(this);
}

void ActorPawn::setInputManager(InputManager *inputManager)
{
    ActorPawn::inputManager = inputManager;
}

InputManager *ActorPawn::getInputManager()
{
    return ActorPawn::inputManager;
}
