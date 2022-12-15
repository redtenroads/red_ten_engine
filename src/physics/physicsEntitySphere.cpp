// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "physicsEntitySphere.h"

#define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
#define JPH_USE_SSE4_1
#define JPH_USE_SSE4_2
#define NDEBUG
#define JPH_PROFILE_ENABLED
#define JPH_DEBUG_RENDERER

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Constraints/SixDOFConstraint.h>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

// All Jolt symbols are in the JPH namespace
using namespace JPH;

// We're also using STL classes in this example
using namespace std;

PhysicsEntitySphere::PhysicsEntitySphere(float radius, float x, float y, float z) : PhysicsEntity(x, y, z)
{
    this->radius = radius;
}

float PhysicsEntitySphere::getRadius()
{
    return radius;
}

const void *PhysicsEntitySphere::getShape(Vector3 scale)
{
    return new SphereShapeSettings(radius * scale.x); // why not
}