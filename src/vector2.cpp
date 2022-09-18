#include "vector2.h"

Vector2::Vector2()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2 Vector2::operator+(Vector2 const &obj)
{
    Vector2 out;
    out.x = x + obj.x;
    out.y = y + obj.y;
    return out;
}

void Vector2::operator+=(Vector2 const &obj)
{
    x += obj.x;
    y += obj.y;
}

Vector2 Vector2::operator*(float f)
{
    Vector2 out;
    out.x = x * f;
    out.y = y * f;
    return out;
}
void Vector2::operator*=(float f)
{
    x *= f;
    y *= f;
}
Vector2 Vector2::operator/(float f)
{
    Vector2 out;
    out.x = x / f;
    out.y = y / f;
    return out;
}
void Vector2::operator/=(float f) {
    x /= f;
    y /= f;
}