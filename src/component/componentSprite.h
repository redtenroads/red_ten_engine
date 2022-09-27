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

    EXPORT void render(Matrix4 &vpMatrix, Transformation *tf);

    EXPORT void setOpacity(float opacity);
    EXPORT float getOpacity();
    EXPORT void setAnchor(float x, float y);
    EXPORT void setAnchor(Vector2 &anchor);
    EXPORT void setTexture(Texture *texture);

    EXPORT void setFramedRender(int framesInLine, int frameRows);
    EXPORT void setFrame(int frame);

    EXPORT Matrix4 getLocalspaceMatrix();

protected:
    void calcFrameRenderSize();

    int framesInLine = 1;
    int framesTotal = 1;
    int currentFrame = 0;
    float frameShift[2] = {0.0f, 0.0f};
    float frameSize[2] = {1.0f, 1.0f};
    float frameRenderSize[2] = {1.0f, 1.0f};

    float opacity = 1.0f;
    Texture *texture = nullptr;
    Shader *shader = nullptr;
    Shader *shaderFramed = nullptr;
    Matrix4 mAnchor;

};
