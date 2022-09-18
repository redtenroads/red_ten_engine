#include "layerActors.h"
#include "matrix4.h"
#include "actorGUIElement.h"
#include <math.h>

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
#include <Jolt/Physics/Body/BodyLock.h>
#include <Jolt/Physics/Body/BodyLockInterface.h>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

// All Jolt symbols are in the JPH namespace
using namespace JPH;

// We're also using STL classes in this examAllHitCollisionCollectorple
using namespace std;

LayerActors::LayerActors(std::string name, int index) : Layer(name, index) {}

void LayerActors::process(float delta)
{
    if (!bProcessingEnabled)
        return;

    for (auto actor = actors.begin(); actor != actors.end(); ++actor)
    {
        (*actor)->onProcess(delta);
        (*actor)->preSyncPhysics();
    }

    if (physicsSystem)
    {
        const int cCollisionSteps = 1;
        const int cIntegrationSubSteps = 1;

        PhysicsSystem *pSystem = (PhysicsSystem *)physicsSystem->system;
        BroadPhase *broadPhase = (BroadPhase *)physicsSystem->broadPhase;

        TempAllocatorImpl *temAllocator = (TempAllocatorImpl *)physicsSystem->tempAllocator;
        JobSystemThreadPool *JobSystem = (JobSystemThreadPool *)physicsSystem->JobSystem;

        // Step the world
        pSystem->Update(delta, cCollisionSteps, cIntegrationSubSteps, temAllocator, JobSystem);

        for (auto actor = actors.begin(); actor != actors.end(); ++actor)
            (*actor)->afterSyncPhysics();

        broadPhase->UpdatePrepare();
    }

    if (bUseSorting)
    {
        actors.sort([](const Actor *player1, const Actor *player2)
                    { return player1->zDepth < player2->zDepth; });
    }

    auto actor = actors.begin();
    while (actor != actors.end())
        if ((*actor)->isMarkedToDestory())
        {
            delete (*actor);
            actor = actors.erase(actor);
        }
        else
            ++actor;
}

void LayerActors::render(View *view)
{
    if (activeCamera && bIsVisible)
    {
        view->prepare();
        activeCamera->prepareToRender(view);
        ActorGUIElement::camera = activeCamera;

        Matrix4 t;
        t.setIdentity();
        t.multiply(*activeCamera->getViewMatrix());
        t.multiply(*activeCamera->getProjectionMatrix());

        for (auto actor = actors.begin(); actor != actors.end(); ++actor)
        {
            if ((*actor)->isVisible())
                (*actor)->onRender(t);
        }

        activeCamera->finishRender();
    }
}

void LayerActors::prepareNewActor(Actor *actor)
{
    if (physicsSystem)
        actor->providePhysicsSystem(physicsSystem);

    actors.push_back(actor);
    actor->setCurrentLayer(this);
    actor->onSpawned();
    actor->updatePhysics();

    if (!activeCamera && actor->implements("Camera"))
    {
        activeCamera = (Camera *)actor;
    }
}

void LayerActors::enablePhisics(Vector3 gravity)
{
    if (!physicsSystem)
    {
        this->gravity = gravity;
        auto newPhysicsSystem = physicsManager->createSystem(gravity, 20 * 1024 * 1024);

        for (auto actor = actors.begin(); actor != actors.end(); ++actor)
        {
            (*actor)->providePhysicsSystem(newPhysicsSystem);
            (*actor)->updatePhysics();
        }
        this->physicsSystem = newPhysicsSystem;
    }
}

void LayerActors::disablePhysics()
{
}

void LayerActors::enableSorting()
{
    bUseSorting = true;
}

void LayerActors::disableSorting()
{
    bUseSorting = false;
}

std::list<Actor *> LayerActors::castRayCollision(Vector3 v1, Vector3 v2)
{
    std::list<Actor *> list;
    if (physicsSystem)
    {
        PhysicsSystem *pSystem = (PhysicsSystem *)physicsSystem->system;
        const BroadPhaseQuery *mBroadPhase = &pSystem->GetBroadPhaseQuery();

        // Create ray
        RayCast ray{
            Vec3(v1.x * SIZE_MULTIPLIER, v1.y * SIZE_MULTIPLIER, v1.z * SIZE_MULTIPLIER),
            Vec3(v2.x * SIZE_MULTIPLIER, v2.y * SIZE_MULTIPLIER, v2.z * SIZE_MULTIPLIER)};

        // Raycast before update
        AllHitCollisionCollector<RayCastBodyCollector> ioCollector;
        mBroadPhase->CastRay(ray, ioCollector);
        if (ioCollector.mHits.size())
            for (auto item = ioCollector.mHits.begin(); item != ioCollector.mHits.end(); ++item)
            {
                BodyLockRead lock(pSystem->GetBodyLockInterface(), item->mBodyID);
                const Body &body = lock.GetBody();
                list.push_back((Actor *)body.GetUserData());
                lock.ReleaseLock();
            }
    }
    return list;
}

std::list<Actor *> LayerActors::castSphereCollision(Vector3 p, float radius)
{
    std::list<Actor *> list;
    if (physicsSystem && physicsSystem->system)
    {
        PhysicsSystem *pSystem = (PhysicsSystem *)physicsSystem->system;

        // auto mBroadPhase = &pSystem->GetBroadPhaseQuery();
        //  shared_lock lock(mMutex);

        float radiusSQ = Square(radius * SIZE_MULTIPLIER);

        BodyIDVector outBodyIDs;
        pSystem->GetBodies(outBodyIDs);

        Vec3 mPosition = Vec3(p.x * SIZE_MULTIPLIER, p.y * SIZE_MULTIPLIER, p.z * SIZE_MULTIPLIER);

        for (BodyID b : outBodyIDs)
        {
            BodyLockRead lock(pSystem->GetBodyLockInterface(), b);
            if (lock.Succeeded())
            {
                const Body &body = lock.GetBody();
                const AABox &bounds = body.GetWorldSpaceBounds();

                if (bounds.GetSqDistanceTo(mPosition) < radiusSQ)
                {
                    if ((Actor *)body.GetUserData())
                        list.push_back((Actor *)body.GetUserData());
                    lock.ReleaseLock();
                }
            }

            /*
                        // Test layer
                        if (inObjectLayerFilter.ShouldCollide(body.GetObjectLayer()))
                        {
                            // Test intersection with box
                            const AABox &bounds = body.GetWorldSpaceBounds();
                            if (bounds.GetSqDistanceTo(inCenter) <= radiusSQ)
                            {
                            }
                        }
                    }
                    */

            /*
                    AllHitCollisionCollector<CollideShapeBodyCollector> ioCollector;
                    mBroadPhase->CollideSphere(
                        Vec3(p.x * SIZE_MULTIPLIER, p.y * SIZE_MULTIPLIER, p.z * SIZE_MULTIPLIER),
                        radius * SIZE_MULTIPLIER, ioCollector);

                    if (ioCollector.HadHit())
                    {
                        for (auto item = ioCollector.mHits.begin(); item != ioCollector.mHits.end(); ++item)
                        {
                            BodyLockRead lock(pSystem->GetBodyLockInterface(), *item);
                            if (lock.Succeeded())
                            {
                                const Body &body = lock.GetBody();
                                float radius_sq = Square(radius * SIZE_MULTIPLIER);
                                const AABox &bounds = body.GetWorldSpaceBounds();
                                if (bounds.GetSqDistanceTo(Vec3(p.x * SIZE_MULTIPLIER, p.y * SIZE_MULTIPLIER, p.z * SIZE_MULTIPLIER)) < radius_sq)
                                {
                                    auto vec3pos = body.GetPosition();
                                    printf(
                                        "B %f %f %f(%f %f %f)\n",
                                        vec3pos.GetX(),
                                        vec3pos.GetY(),
                                        vec3pos.GetZ(),
                                        vec3pos.GetX() / SIZE_MULTIPLIER,
                                        vec3pos.GetY() / SIZE_MULTIPLIER,
                                        vec3pos.GetZ() / SIZE_MULTIPLIER);

                                    printf("R %f <= %f\n",
                                           bounds.GetSqDistanceTo(Vec3(p.x * SIZE_MULTIPLIER, p.y * SIZE_MULTIPLIER, p.z * SIZE_MULTIPLIER)),
                                           radius_sq);

                                    list.push_back((Actor *)body.GetUserData());
                                    lock.ReleaseLock();
                                }
                            }
                        }
                        printf("END\n");
                    }
                    */
        }
    }
    return list;
}

std::list<Actor *> LayerActors::castPointCollision(Vector3 v1)
{
    std::list<Actor *> list;
    if (physicsSystem)
    {
        PhysicsSystem *pSystem = (PhysicsSystem *)physicsSystem->system;
        const BroadPhaseQuery *mBroadPhase = &pSystem->GetBroadPhaseQuery();

        // Raycast before update
        AllHitCollisionCollector<CollideShapeBodyCollector> ioCollector;
        mBroadPhase->CollidePoint(Vec3(v1.x * SIZE_MULTIPLIER, v1.y * SIZE_MULTIPLIER, v1.z * SIZE_MULTIPLIER), ioCollector);
        if (ioCollector.mHits.size())
            for (auto item = ioCollector.mHits.begin(); item != ioCollector.mHits.end(); ++item)
            {
                BodyLockRead lock(pSystem->GetBodyLockInterface(), *item);
                const Body &body = lock.GetBody();
                list.push_back((Actor *)body.GetUserData());
                lock.ReleaseLock();
            }
    }
    return list;
}

std::list<Actor *> *LayerActors::getActorsList()
{
    return &actors;
}

void LayerActors::clear(bool destroyCameras)
{
    if (actors.size() > 0)
    {
        for (auto actor = actors.begin(); actor != actors.end(); ++actor)
        {
            if ((!destroyCameras && !(*actor)->implements("Camera")) || (destroyCameras))
            {
                (*actor)->destroy();
            }
        }
    }
}