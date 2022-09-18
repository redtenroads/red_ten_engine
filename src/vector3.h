#pragma once
#include "matrix4.h"
#include "utils.h"

class Vector3
{
public:
    EXPORT Vector3();
    EXPORT Vector3(float xyz);
    EXPORT Vector3(float x, float y, float z);

    EXPORT Vector3 operator+(Vector3 const &obj);
    EXPORT void operator+=(Vector3 const &obj);
    EXPORT Vector3 operator*(float f);
    EXPORT void operator*=(float f);
    EXPORT Vector3 operator*(Matrix4 const &m);
    EXPORT void operator*=(Matrix4 const &m);
    EXPORT Vector3 operator/(float f);
    EXPORT void operator/=(float f);

    float x, y, z;
};
