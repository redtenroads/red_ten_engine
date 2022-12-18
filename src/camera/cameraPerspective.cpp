// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

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
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    float aspect = (float)view->getWidth() / (float)view->getHeight();
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
    if (view)
    {
        float aspect = (float)view->getWidth() / (float)view->getHeight();
        return (int)(useWidthBasedProportion ? mainLine : mainLine * aspect);
    }
    return 0;
}

int CameraPerspective::getHeight()
{
    if (view)
    {
        float aspect = (float)view->getWidth() / (float)view->getHeight();
        return (int)(useWidthBasedProportion ? mainLine / aspect : mainLine);
    }
    return 0;
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

PointWithDirection CameraPerspective::screenToWorld(float x, float y)
{
    PointWithDirection out;
    if (!view)
        return out;

    float halfWidth = view->getWidth() / 2.0f;
    float halfHeight = view->getHeight() / 2.0f;

    Matrix4 mView = glm::inverse(projectionMatrix * *getViewMatrix());

    Vector4 near = Vector4((x - halfWidth) / halfWidth, -1 * (y - halfHeight) / halfHeight, 0.0f, 1.0);
    Vector4 far = Vector4((x - halfWidth) / halfWidth, -1 * (y - halfHeight) / halfHeight, farDistance, 1.0);

    Vector4 nearResult = mView * near;
    Vector4 farResult = mView * far;
    nearResult /= nearResult.w;
    farResult /= farResult.w;

    out.vDirection = glm::normalize(Vector3(nearResult - farResult));

    out.vPosition = Vector3({nearResult.x,
                             nearResult.y,
                             nearResult.z});

    return out;
}