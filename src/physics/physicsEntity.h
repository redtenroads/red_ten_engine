// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "controller/physicsController.h"
#include "math/math.h"

#define SIZE_MULTIPLIER 0.4f

enum class MotionType : unsigned char
{
    Static,    ///< Non movable
    Kinematic, ///< Movable using velocities only, does not respond to forces
    Dynamic,   ///< Responds to forces as a normal physics object
};

class PhysicsEntity
{
public:
    EXPORT PhysicsEntity(float x, float y, float z);

    EXPORT virtual const void *getShape(Vector3 scale);

    float x = 0, y = 0, z = 0;
};
