// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "component/component.h"
#include "resource/texture.h"
#include "shaders/shader.h"

class ComponentSprite : public Component
{
public:
    EXPORT ComponentSprite();

    EXPORT bool onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage);
    EXPORT int getVertexAmount();

    EXPORT void setOpacity(float opacity);
    EXPORT float getOpacity();
    EXPORT void setAnchor(float x, float y);
    EXPORT void setAnchor(Vector2 &anchor);
    EXPORT void setRelativeScale(float x, float y);
    EXPORT void setRelativeScale(Vector2 &size);
    EXPORT void setTexture(Texture *texture);
    EXPORT void setShader(Shader *shader);

    EXPORT Matrix4 getLocalspaceMatrix();

protected:
    float opacity = 1.0f;
    Texture *texture = nullptr;
    Shader *shader = nullptr;
    Matrix4 mAnchor;

};
