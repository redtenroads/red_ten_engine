// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/geometry.h"
#include "controller/physicsController.h"
#include "physics/physicsEntity.h"
#include "math/math.h"

class PhysicsEntityGeometry : public PhysicsEntity
{
public:
    EXPORT PhysicsEntityGeometry(Geometry *geometry, float x = 0, float y = 0, float z = 0);

    EXPORT Geometry *getGeometry();
    EXPORT const void *getShape(Vector3 scale);

protected:
    Geometry *geometry = nullptr;
};
