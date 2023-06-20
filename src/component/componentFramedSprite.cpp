// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/componentFramedSprite.h"
#include "common/commonShaders.h"
#include "math/glm/gtc/type_ptr.hpp"
#include "opengl/glew.h"
#include "opengl/wglew.h"
#include <math.h>

ComponentFramedSprite::ComponentFramedSprite() : Component()
{
    mAnchor = Matrix4(1.0f);
    setAnchor(0.5f, 0.5f);
    shader = CommonShaders::spriteFrameShader;
    colorMode = ComponentColorMode::Alpha;
}

bool ComponentFramedSprite::onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage)
{
    if (texture)
    {
        Matrix4 mOut = *tf->getModelMatrix() * *transform.getModelMatrix() * mAnchor;

        shader->use(vpMatrix, mOut);
        shader->setOpacity(opacity);

        shader->setFrameShift(frameShift);
        shader->setFrameSize(frameRenderSize);

        texture->bind();
        CommonShaders::spriteMesh->use();
        prepareColorMode();
        return true;
    }
    return false;
}

int ComponentFramedSprite::getVertexAmount()
{
    return 4;
}

void ComponentFramedSprite::setOpacity(float opacity)
{
    this->opacity = fmaxf(fminf(opacity, 1.0f), 0.0f);
}

float ComponentFramedSprite::getOpacity()
{
    return opacity;
}

void ComponentFramedSprite::setAnchor(float x, float y)
{
    mAnchor = glm::translate(Matrix4(1.0f), Vector3(-x, -y, 0.0f));
}

void ComponentFramedSprite::setAnchor(Vector2 &anchor)
{
    mAnchor = glm::translate(Matrix4(1.0f), Vector3(-anchor.x, -anchor.y, 0.0f));
}

void ComponentFramedSprite::setRelativeScale(float x, float y)
{
    if (texture)
    {
        texture->reload();
        transform.setScale((float)texture->getWidth() * frameSize[0] * x, (float)texture->getHeight() * frameSize[1] * y, 1.0f);
    }
    else
    {
        transform.setScale(1.0f, 1.0f, 1.0f);
    }
}

void ComponentFramedSprite::setRelativeScale(Vector2 &size)
{
    if (texture)
    {
        texture->reload();
        transform.setScale((float)texture->getWidth() * frameSize[0] * size.x, (float)texture->getHeight() * frameSize[1] * size.y, 1.0f);
    }
    else
    {
        transform.setScale(1.0f, 1.0f, 1.0f);
    }
}

void ComponentFramedSprite::setTexture(Texture *texture)
{
    this->texture = texture;
    if (texture)
        texture->reload();
    calcFrameRenderSize();
}

void ComponentFramedSprite::setShader(Shader *shader)
{
    this->shader = shader;
}

void ComponentFramedSprite::setFramedRender(int framesInLine, int frameRows)
{
    if (framesInLine == 0)
        framesInLine = 1;
    if (frameRows == 0)
        frameRows = 1;

    this->framesInLine = framesInLine;
    framesTotal = framesInLine * frameRows;

    frameSize[0] = 1.0f / (float)framesInLine;
    frameSize[1] = 1.0f / (float)frameRows;

    setFrame(0);

    if (texture)
    {
        calcFrameRenderSize();
    }
}

void ComponentFramedSprite::setFrame(int frame)
{
    frame = frame % framesTotal;
    frameShift[0] = frameSize[0] * (frame % framesInLine);
    frameShift[1] = frameSize[1] * (frame / framesInLine);
    currentFrame = frame;
}

Matrix4 ComponentFramedSprite::getLocalspaceMatrix()
{
    Matrix4 mModelTransform = *transform.getModelMatrix();
    return mModelTransform * mAnchor;
}

void ComponentFramedSprite::calcFrameRenderSize()
{
    if (texture)
    {
        float pxSize = 1.0f / (float)texture->getWidth();
        float pySize = 1.0f / (float)texture->getHeight();
        frameRenderSize[0] = frameSize[0] - pxSize;
        frameRenderSize[1] = frameSize[1] - pySize;
    }
    else
    {
        frameRenderSize[0] = 1.0f;
        frameRenderSize[1] = 1.0f;
    }
    setRelativeScale(1.0f, 1.0f);
}