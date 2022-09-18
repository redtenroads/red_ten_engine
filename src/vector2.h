#pragma once
#include "utils.h"

class Vector2
{
public:
    EXPORT Vector2();
    EXPORT Vector2(float x, float y);

    EXPORT Vector2 operator+(Vector2 const &obj);
    EXPORT void operator+=(Vector2 const &obj);
    EXPORT Vector2 operator*(float f);
    EXPORT void operator*=(float f);
    EXPORT Vector2 operator/(float f);
    EXPORT void operator/=(float f);

    float x, y;
};
