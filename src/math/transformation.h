// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "math/math.h"


class Transformation
{
public:
    EXPORT void setPosition(Vector3 v);
    EXPORT void setPosition(Vector2 v);
    EXPORT void setPosition(float x, float y, float z);
    EXPORT void setPosition(float x, float y);
    EXPORT void translate(float x, float y, float z);
    EXPORT void translate(float x, float y);
    EXPORT void translate(Vector3 v);
    EXPORT void translate(Vector2 v);
    EXPORT Vector3 getPosition();

    EXPORT void setRotation(Vector3 v);
    EXPORT void setRotation(float x, float y, float z);
    EXPORT void setRotation(float z);
    EXPORT void rotate(float z);
    EXPORT void rotate(float x, float y, float z);
    EXPORT void rotate(Vector3 v);
    EXPORT Vector3 getRotation();
    EXPORT float getZRotation();

    EXPORT void setScale(Vector3 v);
    EXPORT void setScale(Vector2 v);
    EXPORT void setScale(float x, float y, float z);
    EXPORT void setScale(float x, float y);
    EXPORT void setScale(float xy);
    EXPORT Vector3 getScale();

    EXPORT void update();
    EXPORT bool isDirty();
    EXPORT Matrix4 *getModelMatrix();

protected:
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 rotation = {0.0f, 0.0f, 0.0f};
    Vector3 scale = {1.0f, 1.0f, 1.0f};
    Matrix4 mModel;
    bool bIsDirty = true;
};
