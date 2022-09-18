#pragma once
#include "utils.h"

class Matrix4
{
public:
    Matrix4();
    Matrix4(float values[16]);

    void setIdentity();
    void setScaling(float x, float y, float z);
    void setTranslation(float x, float y, float z);
    void setXRotation(float rRad);
    void setYRotation(float rRad);
    void setZRotation(float rRad);
    void multiply(Matrix4 &m);

    float values[16] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0};
};
