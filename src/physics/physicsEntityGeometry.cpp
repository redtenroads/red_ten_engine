// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "physicsEntityGeometry.h"

PhysicsEntityGeometry::PhysicsEntityGeometry(Geometry *geometry, float x, float y, float z) : PhysicsEntity(x, y, z)
{
    this->geometry = geometry;
}

Geometry *PhysicsEntityGeometry::getGeometry()
{
    return geometry;
}

const void *PhysicsEntityGeometry::getShape(Vector3 scale)
{
    if (geometry)
    {
    }
    return nullptr;
}