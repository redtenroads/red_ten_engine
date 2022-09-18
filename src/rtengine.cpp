#include "rtengine.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <cmath>
#include <shellscalingapi.h>

#ifdef _WIN32
#include <windows.h>
#endif

ViewManager *RTEngine::viewManager = nullptr;
StageManager *RTEngine::stageManager = nullptr;
ResourceManager *RTEngine::resourceManager = nullptr;
ShadersManager *RTEngine::shadersManager = nullptr;
PhysicsManager *RTEngine::physicsManager = nullptr;
InputManager *RTEngine::inputManager = nullptr;
SoundManager *RTEngine::soundManager = nullptr;

bool RTEngine::isSDLInitDone = false;

RTEngine::RTEngine()
{
    if (!inputManager)
    {
        inputManager = new InputManager();
        ViewManager::setInputManager(inputManager);
        ActorPawn::setInputManager(inputManager);
    }
    if (!viewManager)
    {
        viewManager = new ViewManager();
    }
    if (!stageManager)
    {
        stageManager = new StageManager();
    }
    if (!resourceManager)
    {
        resourceManager = new ResourceManager();
    }
    if (!soundManager)
    {
        soundManager = new SoundManager();
        ComponentSoundPlayer::setSoundManager(soundManager);
        Camera::setSoundManager(soundManager);
    }
    if (!shadersManager)
    {
        shadersManager = new ShadersManager();
        Component::setShadersManager(shadersManager);
        Actor::setShadersManager(shadersManager);
        Stage::setShadersManager(shadersManager);
        Effect::setShadersManager(shadersManager);
        Layer::setShadersManager(shadersManager);
    }
    if (!physicsManager)
    {
        physicsManager = new PhysicsManager();
        physicsManager->setupPhysics();
        Layer::setPhysicsManager(physicsManager);
        Actor::setPhysicsManager(physicsManager);
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

ViewManager *RTEngine::getViewManager()
{
    return viewManager;
}

StageManager *RTEngine::getStageManager()
{
    return stageManager;
}

ResourceManager *RTEngine::getResourceManager()
{
    return resourceManager;
}

PhysicsManager *RTEngine::getPhysicsManager()
{
    return physicsManager;
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
    return viewManager->getIsExitIntended() | bTerminationRequested;
}
