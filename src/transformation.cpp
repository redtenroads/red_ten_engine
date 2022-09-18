#include "transformation.h"
#include <stdio.h>

void Transformation::setPosition(Vector3 v)
{
    this->position = v;
    bIsDirty = true;
}
void Transformation::setPosition(Vector2 v)
{
    this->position = Vector3(v.x, v.y, 0.0f);
    bIsDirty = true;
}
void Transformation::setPosition(float x, float y, float z)
{
    this->position = Vector3(x, y, z);
    bIsDirty = true;
}
void Transformation::setPosition(float x, float y)
{
    this->position = Vector3(x, y, 0.0f);
    bIsDirty = true;
}

void Transformation::translate(float x, float y, float z)
{
    this->position += Vector3(x, y, z);
    bIsDirty = true;
}
void Transformation::translate(float x, float y)
{
    this->position += Vector3(x, y, 0.0f);
    bIsDirty = true;
}
void Transformation::translate(Vector3 v)
{
    this->position += v;
    bIsDirty = true;
}
void Transformation::translate(Vector2 v)
{
    this->position += Vector3(v.x, v.y, 0.0f);
    bIsDirty = true;
}

Vector3 Transformation::getPosition()
{
    return position;
}

void Transformation::setRotation(Vector3 v)
{
    this->rotation = v;
    bIsDirty = true;
}
void Transformation::setRotation(float x, float y, float z)
{
    this->rotation = Vector3(x, y, z);
    bIsDirty = true;
}
void Transformation::setRotation(float z)
{
    this->rotation = Vector3(0.0f, 0.0f, z);
    bIsDirty = true;
}

void Transformation::rotate(float z)
{
    this->rotation.z += z;
    bIsDirty = true;
}

void Transformation::rotate(float x, float y, float z)
{
    this->rotation += Vector3(x, y, z);
    bIsDirty = true;
}
void Transformation::rotate(Vector3 v)
{
    this->rotation += v;
    bIsDirty = true;
}
Vector3 Transformation::getRotation()
{
    return rotation;
}
float Transformation::getZRotation()
{
    return rotation.z;
}

void Transformation::setScale(Vector3 v)
{
    this->scale = v;
    bIsDirty = true;
}
void Transformation::setScale(Vector2 v)
{
    this->scale = Vector3(v.x, v.y, 1.0f);
    bIsDirty = true;
}
void Transformation::setScale(float x, float y, float z)
{
    this->scale = Vector3(x, y, z);
    bIsDirty = true;
}
void Transformation::setScale(float x, float y)
{
    this->scale = Vector3(x, y, 1.0f);
    bIsDirty = true;
}
void Transformation::setScale(float xy)
{
    this->scale = Vector3(xy, xy, 1.0f);
    bIsDirty = true;
}
Vector3 Transformation::getScale()
{
    return scale;
}

void Transformation::update()
{
    bIsDirty = false;

    Matrix4 r, s, t;

    mModel.setIdentity();

    if (scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f)
    {
        s.setIdentity();
        s.setScaling(scale.x, scale.y, scale.z);
        mModel.multiply(s);
    }

    if (rotation.y != 0.0f)
    {
        r.setIdentity();
        r.setYRotation(rotation.y);
        mModel.multiply(r);
    }

    if (rotation.x != 0.0f)
    {
        r.setIdentity();
        r.setXRotation(rotation.x);
        mModel.multiply(r);
    }

    if (rotation.z != 0.0f)
    {
        r.setIdentity();
        r.setZRotation(rotation.z);
        mModel.multiply(r);
    }

    t.setIdentity();
    t.setTranslation(position.x, position.y, position.z);
    mModel.multiply(t);
}

bool Transformation::isDirty()
{
    return bIsDirty;
}

Matrix4 *Transformation::getModelMatrix()
{
    if (bIsDirty)
        update();

    return &mModel;
}