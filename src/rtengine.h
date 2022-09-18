#pragma once
#include "utils.h"
#include "viewManager.h"
#include "stageManager.h"
#include "resourceManager.h"
#include "shadersManager.h"
#include "physicsManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "camera.h"
#include "cameraOrto.h"
#include "actor.h"
#include "actorPawn.h"
#include "actorGUIElement.h"
#include "component.h"
#include "componentSprite.h"
#include "componentSoundPlayer.h"
#include "componentText.h"
#include "stage.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4.h"
#include "effect.h"
#include "watchable.h"
#include "watcher.h"

class RTEngine
{
protected:
    RTEngine();

    static ViewManager *viewManager;
    static StageManager *stageManager;
    static ResourceManager *resourceManager;
    static ShadersManager *shadersManager;
    static PhysicsManager *physicsManager;
    static InputManager *inputManager;
    static SoundManager *soundManager;

    static bool isSDLInitDone;

    Stage *currentStage = nullptr;

    unsigned int tick = 0;
    int fps = 0, fpsLastCheckTick = 0, fpsCounter = 0;
    bool bTerminationRequested = false;

public:
    EXPORT static RTEngine *createInstance();

    EXPORT static ViewManager *getViewManager();
    EXPORT static StageManager *getStageManager();
    EXPORT static ResourceManager *getResourceManager();
    EXPORT static PhysicsManager *getPhysicsManager();
    EXPORT static InputManager *getInputManager();

    EXPORT void openUrl(const char *url);

    EXPORT float syncFrame();
    EXPORT void terminate();
    EXPORT bool isTerminationIntended();
};
