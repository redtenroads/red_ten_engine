// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "common/effect.h"
#include "opengl/glew.h"
#include <stdio.h>

void Effect::makeFromShaderSource(const char *fragmentShader)
{
    const char *vertexShader =
        "#version 400\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 texCoord;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   texCoord = aTexCoord;\n"
        "}\n";

    this->vertexCode = _strdup(vertexShader);
    this->fragmentCode = _strdup(fragmentShader);
    reload();
}

void Effect::makeFromShaderSource(const char *vertexShader, const char *fragmentShader)
{
    this->vertexCode = _strdup(vertexShader);
    this->fragmentCode = _strdup(fragmentShader);
    reload();
}

int Effect::getIndex()
{
    return index;
}

void Effect::setEnabled(bool state)
{
    this->bIsEnabled = state;
}

bool Effect::isEnabled()
{
    return bIsEnabled;
}

void Effect::setOpacity(float opacity)
{
    this->opacity = opacity;
}

float Effect::getOpacity()
{
    return opacity;
}
