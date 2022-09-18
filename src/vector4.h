#pragma once
#include "matrix4.h"
#include "utils.h"

class Vector4
{
public:
    EXPORT Vector4();
    EXPORT Vector4(float x, float y, float z, float a);

    EXPORT Vector4 operator+(Vector4 const &obj);
    EXPORT void operator+=(Vector4 const &obj);
    EXPORT Vector4 operator*(float f);
    EXPORT void operator*=(float f);
    EXPORT Vector4 operator*(Matrix4 const &m);
    EXPORT void operator*=(Matrix4 const &m);
    EXPORT Vector4 operator/(float f);
    EXPORT void operator/=(float f);

    float x, y, z, a;
};
