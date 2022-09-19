// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "controller/physicsController.h"
#include "physics/physicsEntity.h"
#include "math/math.h"

class PhysicsEntitySphere : public PhysicsEntity
{
public:
    EXPORT PhysicsEntitySphere(float radius, float x, float y, float z);

    EXPORT float getRadius();

    EXPORT const void *getShape(Vector3 scale);



protected:
    float radius;
};
