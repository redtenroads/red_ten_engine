// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"

struct ShaderDescriptor
{
    unsigned int programm;
    unsigned int vbo;
    unsigned int vao;
    unsigned int mTransformLoc;
    unsigned int mViewProjectionLoc;
    unsigned int fOpacityLoc;
    unsigned int v3ColorLoc;
    unsigned int v2TexCoordShift;
    unsigned int v2TexCoordMul;
};

class ShadersController
{
public:
    EXPORT ShadersController();
    EXPORT void compileSysShaders();

    EXPORT void switchShader(ShaderDescriptor *shader);

    EXPORT ShaderDescriptor *getSpriteShader();
    EXPORT ShaderDescriptor *getSpriteFramedShader();
    EXPORT ShaderDescriptor *getScreenShader();
    EXPORT ShaderDescriptor *getEffectShader();
    EXPORT ShaderDescriptor *getClearShader();

    EXPORT unsigned int makeVBO(float *points, int dataSize);
    EXPORT unsigned int makeVAO(unsigned int vbo);
    EXPORT unsigned int makeProgramm(const char *vertexShader, const char *fragmentShader);

    EXPORT void log(unsigned int shader);

protected:
    void compileSpriteShader();
    void compileSpriteFramedShader();
    void compileScreenShader();
    void compileEffectShader();
    void compileClearShader();

    bool bSysShadersCompiled = false;

    ShaderDescriptor *currentShader = nullptr;

    ShaderDescriptor spriteShader;
    ShaderDescriptor spriteFramedShader;
    ShaderDescriptor screenShader;
    ShaderDescriptor effectShader;
    ShaderDescriptor clearShader;
};
