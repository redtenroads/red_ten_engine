// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "shader.h"

class LightningShader : public Shader
{
public:
    EXPORT LightningShader(const char *vertex, const char *fragment);

    EXPORT bool build();
    EXPORT bool use(Matrix4 mViewProjection, Matrix4 mModel);
    EXPORT void setLightColor(float v[3]);
    EXPORT void setLightDirection(float v[3]);
    EXPORT void setAffectDistance(float value);
    EXPORT void setLightSpaceMatrix(Matrix4 mLightSpace);

    unsigned int locMViewProjection;
    unsigned int locMTransform;
    unsigned int locMLightSpace;

    unsigned int locV3LightColor;
    unsigned int locV3LightDirection;
    unsigned int locFAffectDistance;

    unsigned int locTGAlbedoSpec;
    unsigned int locTGNormal;
    unsigned int locTGPosition;
    unsigned int locTShadowMap;

protected:
    const char *vertexCode = nullptr;
    const char *fragmentCode = nullptr;
};
