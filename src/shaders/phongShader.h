// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "shader.h"
#include "resource/texture.h"

enum TextureType
{
    Albedo = 0,
    Normal = 1,
    Specular = 2,
    Emission = 3
};

class PhongShader : public Shader
{
public:
    EXPORT PhongShader();
    EXPORT PhongShader(const char *vertexCode, const char *fragCode, const char *shadowVertexCode, const char *shadowFragCode);
    EXPORT bool build();
    EXPORT void setTexture(TextureType type, Texture *texture);
    EXPORT bool use(Matrix4 mViewProjection, Matrix4 mModel);
    EXPORT bool useShadow(Matrix4 mViewProjection, Matrix4 mModel);

protected:
    void setShaderCode(const char *vertexCode, const char *fragCode, const char *shadowVertexCode, const char *shadowFragCode);

    const char *vertexCode = nullptr;
    const char *fragCode = nullptr;
    const char *shadowVertexCode = nullptr;
    const char *shadowFragCode = nullptr;

    unsigned int locMViewProjection = 0;
    unsigned int locMTransform = 0;
    unsigned int locMNormal = 0;
    unsigned int locTDefuse = 0;
    unsigned int locTSpecular = 0;
    unsigned int locTNormal = 0;
    unsigned int locTEmission = 0;

    unsigned int locShadowMViewProjection = 0;
    unsigned int locShadowMTransform = 0;

    unsigned int tAlbedo = 0;
    unsigned int tNormal = 0;
    unsigned int tSpecular = 0;
    unsigned int tEmission = 0;

    static unsigned int currentProgramm;
    static unsigned int tBlack;
    static unsigned int tGrey;
    static unsigned int tZeroNormal;
};