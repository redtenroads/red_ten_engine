// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "component/component.h"
#include "resource/texture.h"
#include "shaders/shader.h"
#include "resource/resourceMesh.h"

class ComponentMesh : public Component
{
public:
    EXPORT ComponentMesh();

    EXPORT bool onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage);
    EXPORT int getVertexAmount();

    EXPORT void setMesh(ResourceMesh *mesh);
    EXPORT void setShader(Shader *shader);

    EXPORT Matrix4 getLocalspaceMatrix();

protected:
    void clearMeshBasedPhysics();

    ResourceMesh *mesh = nullptr;
    Shader *shader = nullptr;
};
