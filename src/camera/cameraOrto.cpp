// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "camera/cameraOrto.h"
#include "math/glm/gtc/type_ptr.hpp"
#include "opengl/glew.h"
#include <windows.h>

CameraOrto::CameraOrto() : Camera()
{
    registerName("CameraOrto");
}

void CameraOrto::prepareToRender(View *view)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    float aspect = (float)view->getWidth() / (float)view->getHeight();
    float targetWidth = useWidthBasedProportion ? mainLine : mainLine * aspect;
    float targetHeight = useWidthBasedProportion ? mainLine / aspect : mainLine;

    float r = targetWidth / 2.0f;
    float l = -r;
    float b = -targetHeight / 2.0f;
    float t = -b;
    float f = 8000.0f;
    float n = -1.0f;

    projectionMatrix = glm::ortho(l, r, b, t, n, f);
    this->view = view;
}

void CameraOrto::finishRender()
{
}

int CameraOrto::getWidth()
{
    if (view && view->getHeight() > 0 && view->getWidth() > 0)
    {
        float aspect = (float)view->getWidth() / (float)view->getHeight();
        return (int)(useWidthBasedProportion ? mainLine : mainLine * aspect);
    }
    return 0;
}

int CameraOrto::getHeight()
{
    if (view && view->getHeight() && view->getWidth() > 0)
    {
        float aspect = (float)view->getWidth() / (float)view->getHeight();
        return (int)(useWidthBasedProportion ? mainLine / aspect : mainLine);
    }
    return 0;
}

float CameraOrto::getWidthViewProportion()
{
    if (view)
    {
        float aspect = (float)view->getWidth() / (float)view->getHeight();
        float targetWidth = useWidthBasedProportion ? mainLine : mainLine * aspect;
        return targetWidth / (float)view->getWidth();
    }
    return 0;
}

float CameraOrto::getHeightViewProportion()
{
    float aspect = (float)view->getWidth() / (float)view->getHeight();
    float targetHeight = useWidthBasedProportion ? mainLine / aspect : mainLine;
    return targetHeight / (float)view->getHeight();
}

void CameraOrto::setWidthBasedResolution(float width)
{
    useWidthBasedProportion = true;
    mainLine = width;
}

void CameraOrto::setHeightBasedResolution(float height)
{
    useWidthBasedProportion = false;
    mainLine = height;
}