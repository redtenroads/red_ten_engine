// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/componentSprite.h"
#include "opengl/glew.h"
#include "math/glm/gtc/type_ptr.hpp"
#include "common/commonShaders.h"
#include <math.h>

ComponentMesh::ComponentMesh() : Component()
{
    shader = CommonShaders::spriteShader;
}

void ComponentSprite::render(Matrix4 &vpMatrix, Transformation *tf)
{
    if (mesh && shader)
    {
        Matrix4 mTransform = *transform.getModelMatrix();
        Matrix4 mModelTransform = *tf->getModelMatrix() * mTransform;

        Matrix4 mOut = mModelTransform * mAnchor;

        shader->use();

        glUniformMatrix4fv(shader->mViewProjectionLoc, 1, GL_FALSE, value_ptr(vpMatrix));
        glUniformMatrix4fv(shader->mTransformLoc, 1, GL_FALSE, value_ptr(mOut));
        glUniform1f(shader->fOpacityLoc, opacity);

        if (texture)
            glBindTexture(GL_TEXTURE_2D, texture->getGLTextureId());
        mesh->use();
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
}

void ComponentSprite::setShader(Shader *shader)
{
    this->shader = shader;
    if (shader && !shader->isLoaded())
        shader->reload();
}

Matrix4 ComponentSprite::getLocalspaceMatrix()
{
    return *transform.getModelMatrix();
}
