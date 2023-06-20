// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/componentSprite.h"
#include "common/commonShaders.h"
#include "math/glm/gtc/type_ptr.hpp"
#include <math.h>

ComponentSprite::ComponentSprite() : Component()
{
    mAnchor = Matrix4(1.0f);
    setAnchor(0.5f, 0.5f);
    shader = CommonShaders::spriteShader;
    colorMode = ComponentColorMode::Alpha;
}

bool ComponentSprite::onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage)
{
    Matrix4 mOut = *tf->getModelMatrix() * *transform.getModelMatrix() * mAnchor;

    if (shader)
    {
        shader->use(vpMatrix, mOut);
        shader->setOpacity(opacity);
    }

    if (texture)
    {
        texture->bind();
    }

    CommonShaders::spriteMesh->use();
    prepareColorMode();

    return true;
}

int ComponentSprite::getVertexAmount()
{
    return 4;
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

void ComponentSprite::setRelativeScale(float x, float y)
{
    if (texture)
    {
        texture->reload();
        transform.setScale((float)texture->getWidth() * x, (float)texture->getHeight() * y, 1.0f);
    }
    else
    {
        transform.setScale(1.0f, 1.0f, 1.0f);
    }
}

void ComponentSprite::setRelativeScale(Vector2 &size)
{
    if (texture)
    {
        texture->reload();
        transform.setScale((float)texture->getWidth() * size.x, (float)texture->getHeight() * size.y, 1.0f);
    }
    else
    {
        transform.setScale(1.0f, 1.0f, 1.0f);
    }
}

void ComponentSprite::setTexture(Texture *texture)
{
    this->texture = texture;
    if (texture && !texture->isLoaded())
        texture->reload();
    setRelativeScale(1.0f, 1.0f);
}

void ComponentSprite::setShader(Shader *shader)
{
    this->shader = shader;
}

Matrix4 ComponentSprite::getLocalspaceMatrix()
{
    Matrix4 mModelTransform = *transform.getModelMatrix();
    return mModelTransform * mAnchor;
}
