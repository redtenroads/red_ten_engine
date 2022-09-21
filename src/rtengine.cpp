// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "rtengine.h"
#include "common/commonShaders.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <shellscalingapi.h>

#ifdef _WIN32
#include <windows.h>
#endif

ViewController *RTEngine::viewController = nullptr;
StageController *RTEngine::stageController = nullptr;
ResourceController *RTEngine::resourceController = nullptr;
PhysicsController *RTEngine::physicsController = nullptr;
InputController *RTEngine::inputController = nullptr;
SoundController *RTEngine::soundController = nullptr;

bool RTEngine::isSDLInitDone = false;

RTEngine::RTEngine()
{
    if (!inputController)
    {
        inputController = new InputController();
        ViewController::setInputController(inputController);
        ActorPawn::setInputController(inputController);
    }
    if (!viewController)
    {
        viewController = new ViewController();
    }
    if (!stageController)
    {
        stageController = new StageController();
    }
    if (!resourceController)
    {
        resourceController = new ResourceController();
        CommonShaders::resourceController = resourceController;
    }
    if (!soundController)
    {
        soundController = new SoundController();
        ComponentSoundPlayer::setSoundController(soundController);
        Camera::setSoundController(soundController);
    }
    if (!physicsController)
    {
        physicsController = new PhysicsController();
        physicsController->setupPhysics();
        Layer::setPhysicsController(physicsController);
        Actor::setPhysicsController(physicsController);
    }

    tick = SDL_GetTicks();
    fpsLastCheckTick = tick;
}

RTEngine *RTEngine::createInstance()
{
    if (!isSDLInitDone)
    {

#ifdef _WIN32
        SetProcessDPIAware();
#endif

        isSDLInitDone = true;
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
            return nullptr;
        TTF_Init();
    }

    auto instance = new RTEngine();
    return instance;
}

ViewController *RTEngine::getViewController()
{
    return viewController;
}

StageController *RTEngine::getStageController()
{
    return stageController;
}

ResourceController *RTEngine::getResourceController()
{
    return resourceController;
}

PhysicsController *RTEngine::getPhysicsController()
{
    return physicsController;
}

void RTEngine::openUrl(const char *url)
{
    SDL_OpenURL(url);
}

float RTEngine::syncFrame()
{
    unsigned int newTick = SDL_GetTicks();
    float delta = fmin((float)(newTick - tick) / 1000.0f, 0.1f);
    tick = newTick;
    fpsCounter++;

    if (newTick - fpsLastCheckTick > 1000)
    {
        fpsLastCheckTick = newTick;
        fps = fpsCounter;
        fpsCounter = 0;
        printf("FPS %i\n", fps);
    }

    return delta;
}

void RTEngine::terminate()
{
    bTerminationRequested = true;
}

bool RTEngine::isTerminationIntended()
{
    return viewController->getIsExitIntended() | bTerminationRequested;
}
