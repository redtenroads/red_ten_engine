// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "rtengine.h"
#include "common/commonShaders.h"
#include "stage/layerDebug.h"
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
LogController *RTEngine::logController = nullptr;
ConfigController *RTEngine::configController = nullptr;
DebugController *RTEngine::debugController = nullptr;

bool RTEngine::isSDLInitDone = false;

RTEngine::RTEngine(std::string configFilePath)
{
    Config *config = nullptr;
    if (!logController)
    {
        logController = new LogController("log.txt");
        WithLogger::setLogController(logController);
    }
    if (!debugController)
    {
        debugController = new DebugController();
        LayerDebug::setDebugController(debugController);
        WithDebug::setDebugController(debugController);
    }
    if (!configController)
    {
        configController = new ConfigController();
        config = configController->getConfig();
        config->setupByQuality(RenderQuality::Balanced);
        config->setConfigFilePath(configFilePath);
        if (config->loadConfig())
            logController->logff("Configuration loaded\n");
        else
            logController->logff("No configuration found\n");
    }
    if (!inputController)
    {
        inputController = new InputController();
        ViewController::setInputController(inputController);
    }

    if (!viewController)
    {
        viewController = new ViewController(config);
        configController->setViewController(viewController);
    }

    if (!stageController)
    {
        stageController = new StageController();
    }

    if (!resourceController)
    {
        resourceController = new ResourceController();
        LayerDebug::setFont(resourceController->addFont(28));
    }

    if (!soundController)
    {
        soundController = new SoundController(config);
        configController->setSoundController(soundController);
        Camera::setSoundController(soundController);
        SoundPlayer::setSoundController(soundController);
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

RTEngine *RTEngine::createInstance(std::string configFilePath)
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

    auto instance = new RTEngine(configFilePath);
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

InputController *RTEngine::getInputController()
{
    return inputController;
}

ConfigController *RTEngine::getConfigController()
{
    return configController;
}

DebugController *RTEngine::getDebugController()
{
    return debugController;
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
        logController->logConsole("FPS %i", fps);
    }

    soundController->process(delta);
    debugController->process(delta);
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
