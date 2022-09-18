#include "effect.h"
#include "stdio.h"
#include "glew.h"

ShadersManager *Effect::shadersManager = nullptr;

Effect::Effect()
{
}

Effect::~Effect()
{
    if (programm)
        glDeleteProgram(programm);
}

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

    programm = shadersManager->makeProgramm(vertexShader, fragmentShader);
}

void Effect::makeFromShaderSource(const char *vertexShader, const char *fragmentShader)
{
    programm = shadersManager->makeProgramm(vertexShader, fragmentShader);
}

unsigned int Effect::getProgramm()
{
    return programm;
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

int Effect::getUniformLocation(const char *name)
{
    if (programm)
        return glGetUniformLocation(programm, name);
    return 0;
}

void Effect::switchToShader()
{
    if (programm)
        glUseProgram(programm);
}

void Effect::provideFloatValue(int uniform, int amount, float *value)
{
    glUniform1fv(uniform, amount, value);
}

void Effect::provideFloat2Value(int uniform, int amount, float *value)
{
    glUniform2fv(uniform, amount, value);
}

void Effect::provideFloat3Value(int uniform, int amount, float *value)
{
    glUniform3fv(uniform, amount, value);
}

void Effect::provideIntValue(int uniform, int amount, int *value)
{
    glUniform1iv(uniform, amount, value);
}

void Effect::provideInt2Value(int uniform, int amount, int *value)
{
    glUniform2iv(uniform, amount, value);
}

void Effect::provideInt3Value(int uniform, int amount, int *value)
{
    glUniform3iv(uniform, amount, value);
}

void Effect::setShadersManager(ShadersManager *shadersManager)
{
    Effect::shadersManager = shadersManager;
}