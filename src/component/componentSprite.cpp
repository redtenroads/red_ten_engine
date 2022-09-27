// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/componentSprite.h"
#include "common/commonShaders.h"
#include "opengl/glew.h"
#include "math/glm/gtc/type_ptr.hpp"
#include <math.h>

ComponentSprite::ComponentSprite() : Component()
{
    mAnchor = Matrix4(1.0f);
    setAnchor(0.5f, 0.5f);
    shader = CommonShaders::spriteShader;
    shaderFramed = CommonShaders::spriteFrameShader;
    bUseBlendingPhase = true;
}

void ComponentSprite::render(Matrix4 &vpMatrix, Transformation *tf)
{
    if (texture)
    {
        Matrix4 mOut = *tf->getModelMatrix() * *transform.getModelMatrix() * mAnchor;

        Shader *primaryShader = framesTotal == 1 ? shader : shaderFramed;
        primaryShader->use(vpMatrix, mOut);
        primaryShader->setOpacity(opacity);

        primaryShader->setFrameShift(frameShift);
        primaryShader->setFrameSize(frameRenderSize);

        glBindTexture(GL_TEXTURE_2D, texture->getGLTextureId());
        CommonShaders::spriteMesh->use();
        glDrawArrays(GL_QUADS, 0, 4);
    }
}

void ComponentSprite::setOpacity(float opacity)
{
    this->opacity = fmaxf(fminf(opacity, 1.0f), 0.0f);
}

float ComponentSprite::getOpacity()
{
    return opacity;
}

void ComponentSprite::setAnchor(float x, float y)
{
    mAnchor = glm::translate(Matrix4(1.0f), Vector3(-x, -y, 0.0f));
}

void ComponentSprite::setAnchor(Vector2 &anchor)
{
    mAnchor = glm::translate(Matrix4(1.0f), Vector3(-anchor.x, -anchor.y, 0.0f));
}

void ComponentSprite::setTexture(Texture *texture)
{
    this->texture = texture;
    if (texture && !texture->isLoaded())
        texture->reload();
    calcFrameRenderSize();
}

void ComponentSprite::setFramedRender(int framesInLine, int frameRows)
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

void ComponentSprite::setFrame(int frame)
{
    frame = frame % framesTotal;
    frameShift[0] = frameSize[0] * (frame % framesInLine);
    frameShift[1] = frameSize[1] * (frame / framesInLine);
    currentFrame = frame;
}

Matrix4 ComponentSprite::getLocalspaceMatrix()
{
    Matrix4 mModelTransform = *transform.getModelMatrix();
    return mModelTransform * mAnchor;
}

void ComponentSprite::calcFrameRenderSize()
{
    if (texture)
    {
        transform.setScale((float)texture->getWidth() * frameSize[0], (float)texture->getHeight() * frameSize[1], 1.0f);
        float pxSize = 1.0f / (float)texture->getWidth();
        float pySize = 1.0f / (float)texture->getHeight();
        frameRenderSize[0] = frameSize[0] - pxSize;
        frameRenderSize[1] = frameSize[1] - pySize;
    }
    else
    {
        transform.setScale(1.0f, 1.0f, 1.0f);
        frameRenderSize[0] = 1.0f;
        frameRenderSize[1] = 1.0f;
    }
}