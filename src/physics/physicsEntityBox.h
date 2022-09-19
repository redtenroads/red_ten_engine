// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "physicsEntity.h"
#include "math/math.h"

class PhysicsEntityBox : public PhysicsEntity
{
public:
    EXPORT PhysicsEntityBox(float sizeX, float sizeY, float sizeZ, float x, float y, float z);
    EXPORT PhysicsEntityBox(float sizeX, float sizeY, float x, float y, float z);
    EXPORT PhysicsEntityBox(Vector3 size, float x, float y, float z);
    EXPORT PhysicsEntityBox(Vector2 size, float x, float y, float z);

    EXPORT Vector3 getSize();

    EXPORT const void *getShape(Vector3 scale);

protected:
    Vector3 size;
};
