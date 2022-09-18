#pragma once
#include "utils.h"
#include "physicsManager.h"
#include "vector2.h"

#define SIZE_MULTIPLIER 0.02f

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
