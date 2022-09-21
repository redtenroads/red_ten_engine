// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "resource/shader.h"

class Effect : public Shader
{
public:
    EXPORT void makeFromShaderSource(const char *fragmentShader);
    EXPORT void makeFromShaderSource(const char *vertexShader, const char *fragmentShader);

    EXPORT int getIndex();

    EXPORT void setEnabled(bool state);
    EXPORT bool isEnabled();

    EXPORT void setOpacity(float opacity);
    EXPORT float getOpacity();

protected:
    int index = 0;
    bool bIsEnabled = true;
    float opacity = 1.0f;
};
