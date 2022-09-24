#include "camera/cameraPerspective.h"
#include "opengl/glew.h"
#include "math/glm/gtc/type_ptr.hpp"

CameraPerspective::CameraPerspective() : Camera()
{
    registerName("CameraPerspective");
}

void CameraPerspective::prepareToRender(View *view)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_ALPHA_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    float aspect = (float)view->getWidth() / (float)view->getHeight();
    float distance = 45.0f;
    float targetWidth = useWidthBasedProportion ? mainLine : mainLine * aspect;
    float targetHeight = useWidthBasedProportion ? mainLine / aspect : mainLine;

    projectionMatrix = glm::perspective(distance, (float)targetWidth / (float)targetHeight, nearDistance, farDistance);
    this->view = view;
}

void CameraPerspective::finishRender()
{
}

int CameraPerspective::getWidth()
{
    float aspect = (float)view->getWidth() / (float)view->getHeight();
    return (int)(useWidthBasedProportion ? mainLine : mainLine * aspect);
}

int CameraPerspective::getHeight()
{
    float aspect = (float)view->getWidth() / (float)view->getHeight();
    return (int)(useWidthBasedProportion ? mainLine / aspect : mainLine);
}

float CameraPerspective::getWidthViewProportion()
{
    float aspect = (float)view->getWidth() / (float)view->getHeight();
    float targetWidth = useWidthBasedProportion ? mainLine : mainLine * aspect;
    return targetWidth / (float)view->getWidth();
}

float CameraPerspective::getHeightViewProportion()
{
    float aspect = (float)view->getWidth() / (float)view->getHeight();
    float targetHeight = useWidthBasedProportion ? mainLine / aspect : mainLine;
    return targetHeight / (float)view->getHeight();
}

void CameraPerspective::setWidthBasedResolution(float width)
{
    useWidthBasedProportion = true;
    mainLine = width;
}

void CameraPerspective::setHeightBasedResolution(float height)
{
    useWidthBasedProportion = false;
    mainLine = height;
}

void CameraPerspective::setFarDistance(float farDistance)
{
    this->farDistance = farDistance;
}

void CameraPerspective::setNearDistance(float nearDistance)
{
    this->nearDistance = nearDistance;
}

void CameraPerspective::setFov(float fov)
{
    this->fov = fov;
}