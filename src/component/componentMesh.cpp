// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/componentMesh.h"
#include "math/glm/gtc/type_ptr.hpp"
#include "common/commonShaders.h"
#include "opengl/glew.h"
#include <math.h>

ComponentMesh::ComponentMesh() : Component()
{
    shader = nullptr;
}

void ComponentMesh::render(Matrix4 &vpMatrix, Transformation *tf)
{
    if (mesh && shader)
    {
        int drawAmount = mesh->getFloatsAmount();
        if (drawAmount > 0)
        {
            Matrix4 mModelTransform = *tf->getModelMatrix() * *transform.getModelMatrix();

            shader->use(vpMatrix, mModelTransform);
            mesh->use();

            glDrawArrays(GL_TRIANGLES, 0, drawAmount);
        }
    }
}

void ComponentMesh::shadowRender(Matrix4 &vpMatrix, Transformation *tf)
{
    if (mesh && shader)
    {
        int drawAmount = mesh->getFloatsAmount();
        if (drawAmount > 0)
        {
            Matrix4 mModelTransform = *tf->getModelMatrix() * *transform.getModelMatrix();

            shader->useShadow(vpMatrix, mModelTransform);
            mesh->use();

            glDrawArrays(GL_TRIANGLES, 0, drawAmount);
        }
    }
}

void ComponentMesh::setMesh(Mesh *mesh)
{
    this->mesh = mesh;
}

void ComponentMesh::setShader(Shader *shader)
{
    this->shader = shader;
    if (shader && !shader->isReady())
        shader->build();
}

Matrix4 ComponentMesh::getLocalspaceMatrix()
{
    return *transform.getModelMatrix();
}
