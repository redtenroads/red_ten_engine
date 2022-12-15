// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "controller/physicsController.h"
#include "actor/actor.h"
#include <stdio.h>
#include <iostream>

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
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhase.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseQuadTree.h>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

// All Jolt symbols are in the JPH namespace
using namespace JPH;

// We're also using STL classes in this example
using namespace std;

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
    static constexpr BroadPhaseLayer NON_MOVING(0);
    static constexpr BroadPhaseLayer MOVING(1);
    static constexpr BroadPhaseLayer ROOT(2);
    static constexpr uint NUM_LAYERS(3);
};

namespace Layers
{
    static constexpr uint8 NON_MOVING = 0;
    static constexpr uint8 MOVING = 1;
    static constexpr uint8 ROOT = 2;
    static constexpr uint8 NUM_LAYERS = 3;
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
{
public:
    BPLayerInterfaceImpl()
    {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
        mObjectToBroadPhase[Layers::ROOT] = BroadPhaseLayers::ROOT;
    }

    virtual uint GetNumBroadPhaseLayers() const override
    {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override
    {
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char *GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
    {
        switch ((BroadPhaseLayer::Type)inLayer)
        {
        case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
            return "NON_MOVING";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
            return "MOVING";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::ROOT:
            return "ROOT";
        default:
            JPH_ASSERT(false);
            return "INVALID";
        }
    }
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
    BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

// Function that determines if two broadphase layers can collide
static bool MyBroadPhaseCanCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2)
{
    switch (inLayer1)
    {
    case Layers::NON_MOVING:
        return inLayer2 == BroadPhaseLayers::MOVING;

    case Layers::MOVING:
        return inLayer2 != BroadPhaseLayers::ROOT;

    case Layers::ROOT:
        return false;

    default:
        JPH_ASSERT(false);
        return false;
    }
}

// Function that determines if two object layers can collide
static bool MyObjectCanCollide(ObjectLayer inObject1, ObjectLayer inObject2)
{
    switch (inObject1)
    {
    case Layers::NON_MOVING:
        return inObject2 == Layers::MOVING; // Non moving only collides with moving
    case Layers::MOVING:
        return inObject2 != Layers::ROOT;
    case Layers::ROOT:
        return false;
    default:
        JPH_ASSERT(false);
        return false;
    }
};

// An example activation listener
class MyBodyActivationListener : public BodyActivationListener
{
public:
    virtual void OnBodyActivated(const BodyID &inBodyID, uint64 inBodyUserData) override
    {
    }

    virtual void OnBodyDeactivated(const BodyID &inBodyID, uint64 inBodyUserData) override
    {
    }
};

// An example contact listener
class ActorContactListener : public ContactListener
{
public:
    ActorContactListener(PhysicsSystem *physicsSystem)
    {
        this->physicsSystem = physicsSystem;
    }

    virtual void OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
    {
        Actor *actor1 = (Actor *)inBody1.GetUserData();
        Actor *actor2 = (Actor *)inBody2.GetUserData();
        if (actor1 && actor2)
        {
            auto point1 = inManifold.mRelativeContactPointsOn1.begin();
            auto point2 = inManifold.mRelativeContactPointsOn2.begin();

            actor1->onCollide(actor2, Vector3(point1->GetX(), point1->GetY(), point1->GetZ()));
            actor2->onCollide(actor1, Vector3(point2->GetX(), point2->GetY(), point2->GetZ()));
        }
    }

    virtual void OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
    {
        Actor *actor1 = (Actor *)inBody1.GetUserData();
        Actor *actor2 = (Actor *)inBody2.GetUserData();
        if (actor1 && actor2)
        {
            auto point1 = inManifold.mRelativeContactPointsOn1.begin();
            auto point2 = inManifold.mRelativeContactPointsOn2.begin();

            actor1->onCollidePersisted(actor2, Vector3(point1->GetX(), point1->GetY(), point1->GetZ()));
            actor2->onCollidePersisted(actor1, Vector3(point2->GetX(), point2->GetY(), point2->GetZ()));
        }
    }

protected:
    PhysicsSystem *physicsSystem;
    Mutex mStateMutex;
};

PhysicsController::PhysicsController()
{
}

void PhysicsController::setupPhysics()
{
    // Register allocation hook
    RegisterDefaultAllocator();

    // Create a factory
    Factory::sInstance = new Factory();

    // Register all Jolt physics types
    RegisterTypes();
}

PhysicsDescriptor *PhysicsController::createSystem(Vector3 gravity, int sizeOfAllocator)
{
    // We need a temp allocator for temporary allocations during the physics update. We're
    // pre-allocating 10 MB to avoid having to do allocations during the physics update.
    // B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
    // If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
    // malloc / free.

    auto tempAllocator = new TempAllocatorImpl(sizeOfAllocator);
    auto jobSystemThreadPool = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

    // This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    const uint cMaxBodies = 1024;

    // This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
    const uint cNumBodyMutexes = 0;

    // This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
    // body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
    // too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    const uint cMaxBodyPairs = 1024;

    // This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
    // number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
    const uint cMaxContactConstraints = 1024;

    BPLayerInterfaceImpl *broadPhaseLayerInterface = new BPLayerInterfaceImpl();

    PhysicsSystem *physicsSystem = new PhysicsSystem();
    physicsSystem->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *broadPhaseLayerInterface, MyBroadPhaseCanCollide, MyObjectCanCollide);
    PhysicsSettings settings;
    settings.mNumPositionSteps = 4;
    physicsSystem->SetPhysicsSettings(settings);

    // A contact listener gets notified when bodies (are about to) collide, and when they separate again.
    // Note that this is called from a job so whatever you do here needs to be thread safe.
    // Registering one is entirely optional.
    ActorContactListener *contactListener = new ActorContactListener(physicsSystem);
    physicsSystem->SetContactListener(contactListener);

    // A body activation listener gets notified when bodies activate and go to sleep
    // Note that this is called from a job so whatever you do here needs to be thread safe.
    // Registering one is entirely optional.
    MyBodyActivationListener *bodyActivationListener = new MyBodyActivationListener();
    physicsSystem->SetBodyActivationListener(bodyActivationListener);
    physicsSystem->SetGravity(Vec3(gravity.x, gravity.y, gravity.z));

    // Create body
    BodyManager *mBodyManager = new BodyManager();
    mBodyManager->Init(cMaxBodies, 0, *broadPhaseLayerInterface);

    // Crate broadphase
    BroadPhase *mBroadPhase = new BroadPhaseQuadTree();
    mBroadPhase->Init(mBodyManager, *broadPhaseLayerInterface);

    PhysicsDescriptor *newSystem = new PhysicsDescriptor();
    newSystem->system = physicsSystem;
    newSystem->tempAllocator = tempAllocator;
    newSystem->JobSystem = jobSystemThreadPool;

    newSystem->broadPhaseLayerInterface = broadPhaseLayerInterface;
    newSystem->bodyManager = mBodyManager;
    newSystem->broadPhase = mBroadPhase;

    return newSystem;
}

int PhysicsController::getMovingLayerId()
{
    return Layers::MOVING;
}

int PhysicsController::getStaticLayerId()
{
    return Layers::NON_MOVING;
}

int PhysicsController::getRootLayerId()
{
    return Layers::ROOT;
}

void *PhysicsController::getWorld()
{
    return world;
}