// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "os/view.h"
#include "actor/actor.h"
#include "stage/layer.h"
#include "math/math.h"
#include "controller/soundController.h"
#include <list>

class Camera : public Actor
{
public:
    EXPORT Camera();
    EXPORT virtual void prepareToRender(View *view);
    EXPORT virtual void finishRender();
    EXPORT Matrix4 *getProjectionMatrix();
    EXPORT Matrix4 *getViewMatrix();
    EXPORT virtual int getWidth();
    EXPORT virtual int getHeight();
    EXPORT virtual float getWidthViewProportion();
    EXPORT virtual float getHeightViewProportion();
    
    EXPORT void onProcess(float delta);

    EXPORT void setAsListenerCamera();
    EXPORT bool isListenerCamera();

    static void setSoundController(SoundController *soundController);

protected:
    Matrix4 projectionMatrix;

    static SoundController *soundController;
};
