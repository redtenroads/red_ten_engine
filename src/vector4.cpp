#include "vector4.h"

Vector4::Vector4()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    a = 0.0f;
}
Vector4::Vector4(float x, float y, float z, float a)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->a = a;
}

Vector4 Vector4::operator+(Vector4 const &obj)
{
    Vector4 out;
    out.x = x + obj.x;
    out.y = y + obj.y;
    out.z = z + obj.z;
    out.a = a + obj.a;
    return out;
}

void Vector4::operator+=(Vector4 const &obj)
{
    x += obj.x;
    y += obj.y;
    z += obj.z;
    a += obj.a;
}

Vector4 Vector4::operator*(float f)
{
    Vector4 out;
    out.x = x * f;
    out.y = y * f;
    out.z = z * f;
    out.a = a * f;
    return out;
}

void Vector4::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    a *= f;
}

Vector4 Vector4::operator*(Matrix4 const &m)
{
    Vector4 out;
    out.x = this->x * m.values[0] + this->y * m.values[4] + this->z * m.values[8] + this->a * m.values[12];
    out.y = this->x * m.values[1] + this->y * m.values[5] + this->z * m.values[9] + this->a * m.values[13];
    out.z = this->x * m.values[2] + this->y * m.values[6] + this->z * m.values[10] + this->a * m.values[14];
    out.a = this->x * m.values[3] + this->a * m.values[7] + this->a * m.values[11] + this->a * m.values[15];
    return out;
}

void Vector4::operator*=(Matrix4 const &m)
{
    Vector4 out;
    out.x = this->x * m.values[0] + this->y * m.values[4] + this->z * m.values[8] + this->a * m.values[12];
    out.y = this->x * m.values[1] + this->y * m.values[5] + this->z * m.values[9] + this->a * m.values[13];
    out.z = this->x * m.values[2] + this->y * m.values[6] + this->z * m.values[10] + this->a * m.values[14];
    out.a = this->x * m.values[3] + this->a * m.values[7] + this->a * m.values[11] + this->a * m.values[15];
    this->x = out.x;
    this->y = out.y;
    this->z = out.z;
    this->a = out.a;
}

Vector4 Vector4::operator/(float f)
{
    Vector4 out;
    out.x = x / f;
    out.y = y / f;
    out.z = z / f;
    out.a = a / f;
    return out;
}

void Vector4::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    a /= f;
}
