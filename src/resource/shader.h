// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

// supports formats:
// usual: .wav,
// streaming: .ogg

#pragma once
#include "common/utils.h"
#include <string>

class Shader
{
public:
    EXPORT Shader();
    EXPORT Shader(std::string vertexPath, std::string fragmentPath);
    EXPORT Shader(const char *vertex, const char *fragment);

    EXPORT void reload();
    EXPORT bool isLoaded();
    EXPORT bool isPath(std::string vertexPath, std::string fragmentPath);
    EXPORT void use();

    EXPORT int getUniformLocation(const char *name);
    EXPORT void provideFloatValue(int uniform, int amount, float *value);
    EXPORT void provideFloat2Value(int uniform, int amount, float *value);
    EXPORT void provideFloat3Value(int uniform, int amount, float *value);
    EXPORT void provideIntValue(int uniform, int amount, int *value);
    EXPORT void provideInt2Value(int uniform, int amount, int *value);
    EXPORT void provideInt3Value(int uniform, int amount, int *value);

    unsigned int mTransformLoc;
    unsigned int mViewProjectionLoc;
    unsigned int fOpacityLoc;
    unsigned int v3ColorLoc;
    unsigned int v2TexCoordShiftLoc;
    unsigned int v2TexCoordMulLoc;

    static Shader *currentShader;

protected:
    bool compile(const char *vertexShader, const char *fragmentShader);
    void showCompilationError(unsigned int shader);

    std::string vertexPath;
    std::string fragmentPath;
    const char *vertexCode = nullptr;
    const char *fragmentCode = nullptr;

    bool bIsLoaded = false;

    unsigned int programm = 0;
};
