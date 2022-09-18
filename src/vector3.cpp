#include "vector3.h"

Vector3::Vector3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

EXPORT Vector3::Vector3(float xyz)
{
    x = xyz;
    y = xyz;
    z = xyz;
}

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3 Vector3::operator+(Vector3 const &obj)
{
    Vector3 out;
    out.x = x + obj.x;
    out.y = y + obj.y;
    out.z = z + obj.z;
    return out;
}

void Vector3::operator+=(Vector3 const &obj)
{
    x += obj.x;
    y += obj.y;
    z += obj.z;
}

Vector3 Vector3::operator*(float f)
{
    Vector3 out;
    out.x = x * f;
    out.y = y * f;
    out.z = z * f;
    return out;
}
void Vector3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
}

Vector3 Vector3::operator*(Matrix4 const &m)
{
    Vector3 out;
    out.x = this->x * m.values[0] + this->y * m.values[4] + this->z * m.values[8] + m.values[12];
    out.y = this->x * m.values[1] + this->y * m.values[5] + this->z * m.values[9] + m.values[13];
    out.z = this->x * m.values[2] + this->y * m.values[6] + this->z * m.values[10] + m.values[14];
    return out;
}

void Vector3::operator*=(Matrix4 const &m)
{
    Vector3 out;
    out.x = this->x * m.values[0] + this->y * m.values[4] + this->z * m.values[8] + m.values[12];
    out.y = this->x * m.values[1] + this->y * m.values[5] + this->z * m.values[9] + m.values[13];
    out.z = this->x * m.values[2] + this->y * m.values[6] + this->z * m.values[10] + m.values[14];
    this->x = out.x;
    this->y = out.y;
    this->z = out.z;
}

Vector3 Vector3::operator/(float f)
{
    Vector3 out;
    out.x = x / f;
    out.y = y / f;
    out.z = z / f;
    return out;
}
void Vector3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
}