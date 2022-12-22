// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/componentMesh.h"
#include "math/glm/gtc/type_ptr.hpp"
#include "common/commonShaders.h"
#include <math.h>

ComponentMesh::ComponentMesh() : Component()
{
    shader = nullptr;
}

bool ComponentMesh::onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage)
{
    if (mesh && shader)
    {
        Matrix4 mModelTransform = *tf->getModelMatrix() * *transform.getModelMatrix();
        shader->use(vpMatrix, mModelTransform);
        mesh->use();
        return true;
    }
    return false;
}

int ComponentMesh::getVertexAmount()
{
    return mesh ? mesh->getVertexAmount() : 0;
}

void ComponentMesh::setMesh(Mesh *mesh)
{
    if (this->mesh != mesh)
    {
        this->mesh = mesh;
        if (owner)
            owner->childUpdated();
    }
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
