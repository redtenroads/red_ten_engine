// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "math/math.h"
#include "common/utils.h"

class Shader
{
public:
    EXPORT virtual bool build();
    EXPORT virtual bool use(Matrix4 mViewProjection, Matrix4 mModel);

    EXPORT virtual void setOpacity(float value) {}
    EXPORT virtual void setFrameShift(float *shift) {}
    EXPORT virtual void setFrameSize(float *size) {}

    EXPORT bool compile(unsigned short type, const char *code, unsigned int *shader);
    EXPORT bool isReady() { return bIsReady; }

protected:
    void showCompilationError(unsigned int shader);

    static Shader *currentShader;

    bool bIsReady = false;
    unsigned int programm = -1;
};
