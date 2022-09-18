#include "cameraOrto.h"
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

    projectionMatrix.setIdentity();

    projectionMatrix.values[0] = 2.0f / (r - l);
    projectionMatrix.values[12] = -(r + l) / (r - l);
    projectionMatrix.values[5] = 2.0f / (t - b);
    projectionMatrix.values[13] = -(t + b) / (t - b);
    projectionMatrix.values[10] = -2.0f / (f - n);
    projectionMatrix.values[14] = -(f + n) / (f - n);

    shadersManager->switchShader(nullptr);

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