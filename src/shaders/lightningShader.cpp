// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "lightningShader.h"
#include "opengl/glew.h"
#include "math/glm/gtc/type_ptr.hpp"
#include <string.h>

LightningShader::LightningShader(const char *vertex, const char *fragment)
{
    this->vertexCode = _strdup(vertex);
    this->fragmentCode = _strdup(fragment);
}

bool LightningShader::build()
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
            locMLightSpace = glGetUniformLocation(programm, "mlightSpace");

            locV3LightColor = glGetUniformLocation(programm, "lightColor");
            locV3LightDirection = glGetUniformLocation(programm, "lightDir");
            locFAffectDistance = glGetUniformLocation(programm, "affectDistance");

            locTGAlbedoSpec = glGetUniformLocation(programm, "tAlbedoSpec");
            locTGNormal = glGetUniformLocation(programm, "tNormal");
            locTGPosition = glGetUniformLocation(programm, "tPosition");
            locTShadowMap = glGetUniformLocation(programm, "tShadowMap");

            printf("%i %i\n", locMLightSpace, locTShadowMap);

            bIsReady = true;
            return true;
        }
        return false;
    }
    return false;
}

bool LightningShader::use(Matrix4 mViewProjection, Matrix4 mModel)
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

    if (locTGAlbedoSpec != -1)
        glUniform1i(locTGAlbedoSpec, 0);
    if (locTGNormal != -1)
        glUniform1i(locTGNormal, 1);
    if (locTGPosition != -1)
        glUniform1i(locTGPosition, 2);
    if (locTShadowMap != -1)
        glUniform1i(locTShadowMap, 3);

    if (locMViewProjection != -1)
        glUniformMatrix4fv(locMViewProjection, 1, GL_FALSE, value_ptr(mViewProjection));
    if (locMTransform != -1)
        glUniformMatrix4fv(locMTransform, 1, GL_FALSE, value_ptr(mModel));
    return true;
}

void LightningShader::setLightColor(float v[3])
{
    if (locV3LightColor != -1)
        glUniform3fv(locV3LightColor, 1, v);
}

void LightningShader::setLightDirection(float v[3])
{
    if (locV3LightDirection != -1)
        glUniform3fv(locV3LightDirection, 1, v);
}

void LightningShader::setAffectDistance(float value)
{
    if (locFAffectDistance != -1)
        glUniform1f(locFAffectDistance, value);
}
void LightningShader::setLightSpaceMatrix(Matrix4 mLightSpace)
{
    if (locMLightSpace != -1)
        glUniformMatrix4fv(locMLightSpace, 1, GL_FALSE, value_ptr(mLightSpace));
}