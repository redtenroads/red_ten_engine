// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "physicsEntityGeometry.h"

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>
#include <Jolt/Physics/Collision/BackFaceMode.h>
#include <Jolt/Physics/Collision/ActiveEdgeMode.h>
#include <Jolt/Physics/Collision/CollectFacesMode.h>
#include <Jolt/Physics/Collision/Shape/SubShapeID.h>
#include <Jolt/Core/StaticArray.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Geometry/Triangle.h>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

// All Jolt symbols are in the JPH namespace
using namespace JPH;

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
        TriangleList triangles;

        int polyAmount = geometry->getVertexAmount() / 3;
        auto data = geometry->getData();

        for (int i = 0; i < polyAmount; i++)
        {
            const float *base = &data[i * 9];
            Float3 v1 = Float3(base[0] * SIZE_MULTIPLIER, base[1] * SIZE_MULTIPLIER, base[2] * SIZE_MULTIPLIER);
            Float3 v2 = Float3(base[3] * SIZE_MULTIPLIER, base[4] * SIZE_MULTIPLIER, base[5] * SIZE_MULTIPLIER);
            Float3 v3 = Float3(base[6] * SIZE_MULTIPLIER, base[7] * SIZE_MULTIPLIER, base[8] * SIZE_MULTIPLIER);

            triangles.push_back(Triangle(v1, v2, v3, 0));
        }
        return new MeshShapeSettings(triangles);
    }
    return nullptr;
}