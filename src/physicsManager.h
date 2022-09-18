#pragma once
#include "utils.h"
#include "vector3.h"

struct PhysicsDescriptor
{
    void *system;
    void *tempAllocator;
    void *JobSystem;

    void *broadPhaseLayerInterface = nullptr;
    void *broadPhase = nullptr;
    void *bodyManager = nullptr;
};

class PhysicsManager
{
public:
    PhysicsManager();

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
