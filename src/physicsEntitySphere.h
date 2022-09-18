#pragma once
#include "utils.h"
#include "physicsManager.h"
#include "physicsEntity.h"
#include "vector2.h"
#include "vector3.h"

class PhysicsEntitySphere : public PhysicsEntity
{
public:
    EXPORT PhysicsEntitySphere(float radius, float x, float y, float z);

    EXPORT float getRadius();

    EXPORT const void *getShape(Vector3 scale);



protected:
    float radius;
};
