#include "camera.h"
#include "vector4.h"
#include "al.h"
#include "alc.h"

SoundManager *Camera::soundManager = nullptr;

Camera::Camera()
{
    registerName("Camera");
    if (!soundManager->getListenerCamera())
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
    soundManager->setListenerCamera(this);
}

bool Camera::isListenerCamera()
{
    return this == soundManager->getListenerCamera();
}

void Camera::setSoundManager(SoundManager *soundManager)
{
    Camera::soundManager = soundManager;
}