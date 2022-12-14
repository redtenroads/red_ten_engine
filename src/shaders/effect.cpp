// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "effect.h"
#include "opengl/glew.h"
#include <stdio.h>
#include <string>

const char *Effect::internalVertexShader =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   texCoord = aTexCoord;\n"
    "}\n";

Effect::Effect(const char *fragmentShader) : RawShader(internalVertexShader, fragmentShader)
{
    build();
}

Effect::Effect(const char *vertexShader, const char *fragmentShader) : RawShader(vertexShader, fragmentShader)
{
    build();
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

EXPORT float Effect::getOpacity()
{
    return opacity;
}