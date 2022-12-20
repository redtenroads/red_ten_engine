#include "color.h"
#include <string>

Color::Color()
{
    this->r = 0.0f;
    this->g = 0.0f;
    this->b = 0.0f;
    this->a = 1.0f;
}

Color::Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1.0f;
}

Color::Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color &Color::operator=(const Color &other)
{
    if (this != &other) // not a self-assignment
    {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
        this->a = other.a;
    }
    return *this;
}