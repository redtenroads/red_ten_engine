#pragma once
#include "utils.h"
#include "view.h"
#include "actor.h"
#include "layer.h"
#include "matrix4.h"
#include "soundManager.h"
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


    static void setSoundManager(SoundManager *soundManager);

protected:
    Matrix4 projectionMatrix;

    static SoundManager *soundManager;
};
