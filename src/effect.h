#pragma once
#include "utils.h"
#include "shadersManager.h"

class Effect
{
public:
    EXPORT Effect();
    EXPORT ~Effect();

    EXPORT void makeFromShaderSource(const char *fragmentShader);
    EXPORT void makeFromShaderSource(const char *vertexShader, const char *fragmentShader);

    EXPORT unsigned int getProgramm();
    EXPORT int getIndex();

    EXPORT void setEnabled(bool state);
    EXPORT bool isEnabled();

    EXPORT void setOpacity(float opacity);
    EXPORT float getOpacity();

    EXPORT void switchToShader();

    EXPORT int getUniformLocation(const char *name);
    EXPORT void provideFloatValue(int uniform, int amount, float *value);
    EXPORT void provideFloat2Value(int uniform, int amount, float *value);
    EXPORT void provideFloat3Value(int uniform, int amount, float *value);
    EXPORT void provideIntValue(int uniform, int amount, int *value);
    EXPORT void provideInt2Value(int uniform, int amount, int *value);
    EXPORT void provideInt3Value(int uniform, int amount, int *value);

    static void setShadersManager(ShadersManager *shadersManager);

protected:
    unsigned int programm = 0;
    int index = 0;
    static ShadersManager *shadersManager;
    bool bIsEnabled = true;
    float opacity = 1.0f;
};
