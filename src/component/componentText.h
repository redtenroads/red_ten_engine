// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once

#include "common/utils.h"
#include "common/withLogger.h"
#include "component/component.h"
#include "resource/texture.h"
#include "resource/resourceFont.h"

class ComponentText : public Component, WithLogger
{
public:
    EXPORT ComponentText();
    EXPORT ~ComponentText();

    EXPORT bool onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage);
    EXPORT virtual int getVertexAmount();

    EXPORT void setOpacity(float opacity);
    EXPORT float getOpacity();
    EXPORT void setAnchor(float x, float y);
    EXPORT void setAnchor(Vector2 &anchor);

    EXPORT void setFont(ResourceFont *font);
    EXPORT void setColor(unsigned char r, unsigned char g, unsigned char b);
    EXPORT void setText(std::string string);
    EXPORT std::string getText();
    EXPORT void rebuildString();

    EXPORT int getWidth();
    EXPORT int getHeight();

    EXPORT Matrix4 getLocalspaceMatrix();

protected:
    unsigned char r = 255, g = 255, b = 255;
    std::string string = "";
    bool isStringDirty = true;
    float opacity = 1.0f;
    unsigned int textureID = 0;
    Matrix4 mAnchor;
    ResourceFont *font = nullptr;

    int textTextureWidth = 0, textTextureHeight = 0;
};
