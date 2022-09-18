#include "matrix4.h"
#include <string>
#include <math.h>
#include <string.h>

Matrix4::Matrix4()
{
}

Matrix4::Matrix4(float values[16])
{
    memcpy(this->values, values, 16 * sizeof(float));
}

void Matrix4::setIdentity()
{
    for (int i = 0; i < 16; i++)
        values[i] = 0.0f;
    values[0] = 1.0f;
    values[5] = 1.0f;
    values[10] = 1.0f;
    values[15] = 1.0f;
}

void Matrix4::setScaling(float x, float y, float z)
{
    for (int i = 0; i < 16; i++)
        values[i] = 0.0f;
    values[0] = x;
    values[5] = y;
    values[10] = z;
    values[15] = 1.0f;
}

void Matrix4::setTranslation(float x, float y, float z)
{
    setIdentity();
    values[12] = x;
    values[13] = y;
    values[14] = z;
}

void Matrix4::setXRotation(float rRad)
{
    float cosR = cosf(rRad);
    float sinR = sinf(rRad);
    setIdentity();
    values[5] = cosR;
    values[6] = -sinR;
    values[9] = sinR;
    values[10] = cosR;
}

void Matrix4::setYRotation(float rRad)
{
    float cosR = cosf(rRad);
    float sinR = sinf(rRad);
    setIdentity();
    values[0] = cosR;
    values[2] = sinR;
    values[8] = -sinR;
    values[10] = cosR;
}

void Matrix4::setZRotation(float rRad)
{
    float cosR = cosf(rRad);
    float sinR = sinf(rRad);
    setIdentity();
    values[0] = cosR;
    values[4] = -sinR;
    values[1] = sinR;
    values[5] = cosR;
}

void Matrix4::multiply(Matrix4 &m)
{

    float newValues[16];
    newValues[0] = values[0] * m.values[0] + values[1] * m.values[4] + values[2] * m.values[8] + values[3] * m.values[12];
    newValues[1] = values[0] * m.values[1] + values[1] * m.values[5] + values[2] * m.values[9] + values[3] * m.values[13];
    newValues[2] = values[0] * m.values[2] + values[1] * m.values[6] + values[2] * m.values[10] + values[3] * m.values[14];
    newValues[3] = values[0] * m.values[3] + values[1] * m.values[7] + values[2] * m.values[11] + values[3] * m.values[15];
    newValues[4] = values[4] * m.values[0] + values[5] * m.values[4] + values[6] * m.values[8] + values[7] * m.values[12];
    newValues[5] = values[4] * m.values[1] + values[5] * m.values[5] + values[6] * m.values[9] + values[7] * m.values[13];
    newValues[6] = values[4] * m.values[2] + values[5] * m.values[6] + values[6] * m.values[10] + values[7] * m.values[14];
    newValues[7] = values[4] * m.values[3] + values[5] * m.values[7] + values[6] * m.values[11] + values[7] * m.values[15];
    newValues[8] = values[8] * m.values[0] + values[9] * m.values[4] + values[10] * m.values[8] + values[11] * m.values[12];
    newValues[9] = values[8] * m.values[1] + values[9] * m.values[5] + values[10] * m.values[9] + values[11] * m.values[13];
    newValues[10] = values[8] * m.values[2] + values[9] * m.values[6] + values[10] * m.values[10] + values[11] * m.values[14];
    newValues[11] = values[8] * m.values[3] + values[9] * m.values[7] + values[10] * m.values[11] + values[11] * m.values[15];
    newValues[12] = values[12] * m.values[0] + values[13] * m.values[4] + values[14] * m.values[8] + values[15] * m.values[12];
    newValues[13] = values[12] * m.values[1] + values[13] * m.values[5] + values[14] * m.values[9] + values[15] * m.values[13];
    newValues[14] = values[12] * m.values[2] + values[13] * m.values[6] + values[14] * m.values[10] + values[15] * m.values[14];
    newValues[15] = values[12] * m.values[3] + values[13] * m.values[7] + values[14] * m.values[11] + values[15] * m.values[15];
    memcpy(this->values, newValues, 16 * sizeof(float));
}
