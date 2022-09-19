// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "math/math.h"

struct PhysicsDescriptor
{
    void *system;
    void *tempAllocator;
    void *JobSystem;

    void *broadPhaseLayerInterface = nullptr;
    void *broadPhase = nullptr;
    void *bodyManager = nullptr;
};

class PhysicsController
{
public:
    PhysicsController();

    void setupPhysics();
    PhysicsDescriptor *createSystem(Vector3 gravity, int sizeOfAllocator);

    int getMovingLayerId();
    int getStaticLayerId();
    int getRootLayerId();

    void *getWorld();

protected:
    void *world = nullptr;
    float sizeMultiplier = 0.0f;
};
