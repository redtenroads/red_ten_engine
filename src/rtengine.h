// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "actor/actor.h"
#include "actor/actorPawn.h"
#include "actor/actorGUIElement.h"
#include "camera/camera.h"
#include "camera/cameraOrto.h"
#include "controller/viewController.h"
#include "controller/stageController.h"
#include "controller/resourceController.h"
#include "controller/shadersController.h"
#include "controller/physicsController.h"
#include "controller/inputController.h"
#include "controller/soundController.h"
#include "component/component.h"
#include "component/componentSprite.h"
#include "component/componentSoundPlayer.h"
#include "component/componentText.h"
#include "stage/stage.h"
#include "math/math.h"
#include "shaders/effect.h"
#include "actor/watchable.h"
#include "actor/watcher.h"

class RTEngine
{
protected:
    RTEngine();

    static ViewController *viewController;
    static StageController *stageController;
    static ResourceController *resourceController;
    static ShadersController *shadersController;
    static PhysicsController *physicsController;
    static InputController *inputController;
    static SoundController *soundController;

    static bool isSDLInitDone;

    Stage *currentStage = nullptr;

    unsigned int tick = 0;
    int fps = 0, fpsLastCheckTick = 0, fpsCounter = 0;
    bool bTerminationRequested = false;

public:
    EXPORT static RTEngine *createInstance();

    EXPORT static ViewController *getViewController();
    EXPORT static StageController *getStageController();
    EXPORT static ResourceController *getResourceController();
    EXPORT static PhysicsController *getPhysicsController();
    EXPORT static InputController *getInputController();

    EXPORT void openUrl(const char *url);

    EXPORT float syncFrame();
    EXPORT void terminate();
    EXPORT bool isTerminationIntended();
};
