// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "shader.h"
#include "opengl/glew.h"

Shader *Shader::currentShader = nullptr;

bool Shader::build()
{
    return false;
}

bool Shader::use(Matrix4 mViewProjection, Matrix4 mModel)
{
    return false;
}

bool Shader::compile(unsigned short type, const char *code, unsigned int *shader)
{
    int isCompiled;
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &code, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        showCompilationError(*shader);
        return false;
    }
    return true;
}

void Shader::showCompilationError(unsigned int shader)
{
    char log[2048];
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    printf("Shader Compilation Error:\n%s\n", log);
}