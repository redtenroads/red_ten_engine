// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "camera/camera.h"
#include "math/math.h"
#include <al.h>
#include <alc.h>

SoundController *Camera::soundController = nullptr;

Camera::Camera()
{
    registerName("Camera");
    if (!soundController->getListenerCamera())
    {
        setAsListenerCamera();
    }
}

void Camera::prepareToRender(View *view)
{
}

void Camera::finishRender()
{
}

Matrix4 *Camera::getProjectionMatrix()
{
    return &projectionMatrix;
}

Matrix4 *Camera::getViewMatrix()
{
    return transform.getModelMatrix();
}

int Camera::getWidth()
{
    return 0;
}

int Camera::getHeight()
{
    return 0;
}

float Camera::getWidthViewProportion()
{
    return 0;
}

float Camera::getHeightViewProportion()
{
    return 0;
}

void Camera::onProcess(float delta)
{
    if (isListenerCamera())
    {
        ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
        auto position = transform.getPosition();
        alListener3f(AL_POSITION, position.x, position.y, position.z);
        alListener3f(AL_VELOCITY, 0, 0, 0);
        alListenerfv(AL_ORIENTATION, listenerOri);
    }
}

void Camera::setAsListenerCamera()
{
    soundController->setListenerCamera(this);
}

bool Camera::isListenerCamera()
{
    return this == soundController->getListenerCamera();
}

void Camera::setSoundController(SoundController *soundController)
{
    Camera::soundController = soundController;
}