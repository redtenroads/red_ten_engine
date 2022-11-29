// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "rawShader.h"
#include "opengl/glew.h"
#include "math/glm/gtc/type_ptr.hpp"
#include <string.h>

RawShader::RawShader(const char *vertex, const char *fragment)
{
    this->vertexCode = _strdup(vertex);
    this->fragmentCode = _strdup(fragment);
}

bool RawShader::build()
{
    if (vertexCode && fragmentCode)
    {
        unsigned int vertexShader = 0, fragmentShader = 0;
        if (!compile(GL_VERTEX_SHADER, vertexCode, &vertexShader))
            return false;

        if (!compile(GL_FRAGMENT_SHADER, fragmentCode, &fragmentShader))
            return false;

        programm = glCreateProgram();
        if (programm != -1)
        {
            glAttachShader(programm, fragmentShader);
            glAttachShader(programm, vertexShader);
            glLinkProgram(programm);

            locMViewProjection = glGetUniformLocation(programm, "mViewProjection");
            locMTransform = glGetUniformLocation(programm, "mTransform");

            locV2TexCoordShift = glGetUniformLocation(programm, "aTexCoordShift");
            locV2TexCoordMul = glGetUniformLocation(programm, "aTexCoordMul");
            locFOpacity = glGetUniformLocation(programm, "fOpacity");
            locV3AmbientColor = glGetUniformLocation(programm, "vf3ambientColor");

            bIsReady = true;
            return true;
        }
        return false;
    }
    return false;
}

bool RawShader::use(Matrix4 mViewProjection, Matrix4 mModel)
{
    if (!bIsReady)
        build();
    if (!bIsReady)
        return false;

    if (currentShader != this)
    {
        currentShader = this;
        glUseProgram(programm);
    }

    if (locMViewProjection != -1)
        glUniformMatrix4fv(locMViewProjection, 1, GL_FALSE, value_ptr(mViewProjection));
    if (locMTransform != -1)
        glUniformMatrix4fv(locMTransform, 1, GL_FALSE, value_ptr(mModel));
    return true;
}

void RawShader::setOpacity(float value)
{
    glUniform1f(locFOpacity, value);
}

void RawShader::setFrameShift(float *shift)
{
    glUniform2fv(locV2TexCoordShift, 1, shift);
}

void RawShader::setFrameSize(float *size)
{
    glUniform2fv(locV2TexCoordMul, 1, size);
}

int RawShader::getUniformLocation(const char *name)
{
    if (programm != -1)
        return glGetUniformLocation(programm, name);
    return 0;
}

void RawShader::provideFloatValue(int uniform, int amount, float *value)
{
    if (programm != -1)
        glUniform1fv(uniform, amount, value);
}

void RawShader::provideFloat2Value(int uniform, int amount, float *value)
{
    if (programm != -1)
        glUniform2fv(uniform, amount, value);
}

void RawShader::provideFloat3Value(int uniform, int amount, float *value)
{
    if (programm != -1)
        glUniform3fv(uniform, amount, value);
}

void RawShader::provideIntValue(int uniform, int amount, int *value)
{
    if (programm != -1)
        glUniform1iv(uniform, amount, value);
}

void RawShader::provideInt2Value(int uniform, int amount, int *value)
{
    if (programm != -1)
        glUniform2iv(uniform, amount, value);
}

void RawShader::provideInt3Value(int uniform, int amount, int *value)
{
    if (programm != -1)
        glUniform3iv(uniform, amount, value);
}
