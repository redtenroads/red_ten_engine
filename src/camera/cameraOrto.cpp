#include "cameraOrto.h"
#include "math/glm/gtc/type_ptr.hpp"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

CameraOrto::CameraOrto() : Camera()
{
    registerName("CameraOrto");
}

void CameraOrto::prepareToRender(View *view)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

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
    shadersController->switchShader(nullptr);
    this->view = view;
}

void CameraOrto::finishRender()
{
}

int CameraOrto::getWidth()
{
    float aspect = (float)view->getWidth() / (float)view->getHeight();
    return (int)(useWidthBasedProportion ? mainLine : mainLine * aspect);
}

int CameraOrto::getHeight()
{
    float aspect = (float)view->getWidth() / (float)view->getHeight();
    return (int)(useWidthBasedProportion ? mainLine / aspect : mainLine);
}

float CameraOrto::getWidthViewProportion()
{
    float aspect = (float)view->getWidth() / (float)view->getHeight();
    float targetWidth = useWidthBasedProportion ? mainLine : mainLine * aspect;
    return targetWidth / (float)view->getWidth();
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