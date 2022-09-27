// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "shaders/rawShader.h"

class Effect : public RawShader
{
public:
    EXPORT Effect(const char *fragmentShader);
    EXPORT Effect(const char *vertexShader, const char *fragmentShader);

    EXPORT int getIndex();

    EXPORT void setEnabled(bool state);
    EXPORT bool isEnabled();

    EXPORT void setOpacity(float opacity);
    EXPORT float getOpacity();

protected:
    int index = 0;
    bool bIsEnabled = true;
    float opacity = 1.0f;
    static const char *internalVertexShader;
};
