// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "shader.h"

class RawShader : public Shader
{
public:
    EXPORT RawShader(const char *vertex, const char *fragment);

    EXPORT bool build();
    EXPORT bool use();
    EXPORT bool use(Matrix4 mViewProjection, Matrix4 mModel);

    EXPORT void setOpacity(float value);
    EXPORT void setFrameShift(float *shift);
    EXPORT void setFrameSize(float *size);

    EXPORT int getUniformLocation(const char *name);
    EXPORT void provideFloatValue(int uniform, int amount, float *value);
    EXPORT void provideFloat2Value(int uniform, int amount, float *value);
    EXPORT void provideFloat3Value(int uniform, int amount, float *value);
    EXPORT void provideFloat4Value(int uniform, int amount, float *value);
    EXPORT void provideIntValue(int uniform, int amount, int *value);
    EXPORT void provideInt2Value(int uniform, int amount, int *value);
    EXPORT void provideInt3Value(int uniform, int amount, int *value);
    EXPORT void provideInt4Value(int uniform, int amount, int *value);

    unsigned int locMViewProjection;
    unsigned int locMTransform;
    unsigned int locV2TexCoordShift;
    unsigned int locV2TexCoordMul;
    unsigned int locFOpacity;
    unsigned int locV3AmbientColor;

protected:
    const char *vertexCode = nullptr;
    const char *fragmentCode = nullptr;
};
