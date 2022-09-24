// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/componentMesh.h"
#include "opengl/glew.h"
#include "math/glm/gtc/type_ptr.hpp"
#include "common/commonShaders.h"
#include <math.h>

ComponentMesh::ComponentMesh() : Component()
{
    shader = CommonShaders::meshShader;
}

void ComponentMesh::render(Matrix4 &vpMatrix, Transformation *tf)
{
    if (mesh && shader)
    {
        int drawAmount = mesh->getFloatsAmount();
        if (drawAmount > 0)
        {
            Matrix4 mModelTransform = *tf->getModelMatrix() * *transform.getModelMatrix();

            shader->use();

            glUniformMatrix4fv(shader->mViewProjectionLoc, 1, GL_FALSE, value_ptr(vpMatrix));
            glUniformMatrix4fv(shader->mTransformLoc, 1, GL_FALSE, value_ptr(mModelTransform));

            if (texture)
                glBindTexture(GL_TEXTURE_2D, texture->getGLTextureId());

            mesh->use();
            glDrawArrays(GL_TRIANGLES, 0, drawAmount);
        }
    }
}

void ComponentMesh::setMesh(Mesh *mesh)
{
    this->mesh = mesh;
}

void ComponentMesh::setTexture(Texture *texture)
{
    this->texture = texture;
    if (texture && !texture->isLoaded())
        texture->reload();
}

void ComponentMesh::setShader(Shader *shader)
{
    this->shader = shader;
    if (shader && !shader->isLoaded())
        shader->reload();
}

Matrix4 ComponentMesh::getLocalspaceMatrix()
{
    return *transform.getModelMatrix();
}
