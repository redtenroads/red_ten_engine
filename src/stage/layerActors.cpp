// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "stage/layerActors.h"
#include "math/math.h"
#include "actor/actorGUIElement.h"
#include "opengl/glew.h"
#include "opengl/wglew.h"
#include "common/commonShaders.h"
#include "component/componentLight.h"
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
        (*actor)->process(delta);
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

    auto actor = actors.begin();
    while (actor != actors.end())
        if ((*actor)->isDestroyed())
        {
            delete (*actor);
            actor = actors.erase(actor);
        }
        else
            ++actor;

    if (bUseSorting)
    {
        actors.sort([](const Actor *player1, const Actor *player2)
                    { return player1->zDepth < player2->zDepth; });
    }
}

void LayerActors::render(View *view)
{
    Matrix4 m1, m2;
    auto renderer = view->getRenderer();
    // Clear light renderer
    renderer->setupLightning();

    // Clear deffered buffer and set this as primary
    renderer->setupNewFrame();

    std::vector<Component *> sceneLights;
    std::vector<Actor *> blends;
    std::vector<Actor *> shadowCasters;

    if (!activeCamera || !bIsVisible)
        return;

    Matrix4 mView = *activeCamera->getViewMatrix();
    Matrix4 mProjectionView = *activeCamera->getProjectionMatrix() * mView;

    // G Buffer phase
    activeCamera->prepareToRender(view);

    for (auto actor = actors.begin(); actor != actors.end(); ++actor)
    {
        if ((*actor)->isVisible())
        {
            (*actor)->onRender(mProjectionView, &sceneLights);
            if ((*actor)->hasBlended())
                blends.push_back(*actor);
            shadowCasters.push_back(*actor);
        }
    }
    activeCamera->finishRender();

    // light phase
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_ALPHA_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    auto initialLightningShader = CommonShaders::initialLightningShader;
    renderer->setupLightning(false);
    initialLightningShader->use(m1, m2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->getAlbedoTexture());
    glUniform3fv(initialLightningShader->locV3AmbientColor, 1, ambientColor);
    CommonShaders::screenMesh->use();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glDrawArrays(GL_QUADS, 0, 4);

    // Lightning phase
    if (sceneLights.size() > 0)
    {
        bool setupLightningFrame = true;
        for (auto light = sceneLights.begin(); light != sceneLights.end(); ++light)
        {
            // shadow preparation phase
            if ((*light)->isUsingShadowPhase())
            {
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);
                if (renderer->getShadowMapSize() < 2048)
                    glFrontFace(GL_CW);
                glDisable(GL_BLEND);

                // prepare shadowed render
                Matrix4 mLightViewProjection = (*light)->preparePreShadowPhase(activeCamera->transform.getPosition());
                renderer->setupShadowHQ();

                for (auto actor = shadowCasters.begin(); actor != shadowCasters.end(); ++actor)
                {
                    (*actor)->onRenderShadowed(mLightViewProjection);
                }
                setupLightningFrame = true;

                if (renderer->getShadowMapSize() < 2048)
                    glFrontFace(GL_CCW);
            }
            if (setupLightningFrame)
            {
                setupLightningFrame = false;
                renderer->setupLightning(false);

                glDisable(GL_DEPTH_TEST);
                glDisable(GL_CULL_FACE);
                glEnable(GL_BLEND);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, renderer->getAlbedoTexture());

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, renderer->getNormalTexture());

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, renderer->getPositionTexture());
            }

            (*light)->renderLightPhase(mProjectionView, renderer->getShadowTexture());
        }
    }

    // Blending phase
    renderer->setupLightning(false);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (blends.size() > 0)
    {
        for (auto actor = blends.begin(); actor != blends.end(); ++actor)
        {
            (*actor)->onRenderBlended(mProjectionView);
        }
    }

    // Final phase
    view->useFrameBuffer();
    glEnable(GL_BLEND);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->getLightningTexture());
    CommonShaders::screenShader->use(m1, m2);
    CommonShaders::screenMesh->use();

    glDrawArrays(GL_QUADS, 0, 4);
}

void LayerActors::prepareNewActor(Actor *actor)
{
    if (physicsSystem)
        actor->providePhysicsSystem(physicsSystem);

    actors.push_back(actor);
    actor->setCurrentLayer(this);
    actor->onSpawned();

    if (!activeCamera && actor->implements("Camera"))
    {
        activeCamera = (Camera *)actor;
    }
}

void LayerActors::enableCollisions()
{
    enablePhisics(Vector3(0.0f, 0.0f, 0.0f));
}

void LayerActors::enablePhisics(Vector3 gravity)
{
    if (!physicsSystem)
    {
        this->gravity = gravity;
        auto newPhysicsSystem = physicsController->createSystem(gravity, 20 * 1024 * 1024);

        for (auto actor = actors.begin(); actor != actors.end(); ++actor)
            (*actor)->providePhysicsSystem(newPhysicsSystem);

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

RayCollision LayerActors::castSingleRayCollision(Vector3 v1, Vector3 v2, int channelId)
{
    if (physicsSystem)
    {
        PhysicsSystem *pSystem = (PhysicsSystem *)physicsSystem->system;
        const NarrowPhaseQuery *mNarrowPhase = &pSystem->GetNarrowPhaseQuery();

        // Create ray
        RRayCast ray{
            Vec3(v1.x * SIZE_MULTIPLIER, v1.y * SIZE_MULTIPLIER, v1.z * SIZE_MULTIPLIER),
            Vec3(v2.x * SIZE_MULTIPLIER, v2.y * SIZE_MULTIPLIER, v2.z * SIZE_MULTIPLIER)};

        RayCastSettings ray_settings;
        ClosestHitCollisionCollector<CastRayCollector> collector;
        IgnoreMultipleBodiesFilter body_filter;

        // Raycast before update
        mNarrowPhase->CastRay(ray, ray_settings, collector, {}, {}, body_filter);

        if (collector.HadHit())
        {
            BodyLockRead lock(pSystem->GetBodyLockInterface(), collector.mHit.mBodyID);
            const Body &body = lock.GetBody();
            Actor *actor = (Actor *)body.GetUserData();
            if (actor && actor->hasCollisionChannel(channelId))
            {
                auto v = ray.GetPointOnRay(collector.mHit.mFraction);
                RayCollision collision({Vector3(v.GetX() / SIZE_MULTIPLIER, v.GetY() / SIZE_MULTIPLIER, v.GetZ() / SIZE_MULTIPLIER), (Actor *)body.GetUserData(), true});
                lock.ReleaseLock();
                return collision;
            }
        }
    }
    return RayCollision({Vector3(0), nullptr, false});
}

std::list<RayCollision> LayerActors::castSphereCollision(Vector3 p, float radius, int channelId)
{
    std::list<RayCollision> list;
    if (physicsSystem && physicsSystem->system)
    {
        PhysicsSystem *pSystem = (PhysicsSystem *)physicsSystem->system;

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
                    Actor *actor = (Actor *)body.GetUserData();
                    if (actor && actor->hasCollisionChannel(channelId))
                    {
                        RayCollision collision({p, (Actor *)body.GetUserData()});
                        if ((Actor *)body.GetUserData())
                            list.push_back(collision);
                        lock.ReleaseLock();
                    }
                }
            }
        }
    }
    return list;
}

std::list<RayCollision> LayerActors::castPointCollision(Vector3 v1, int channelId)
{
    std::list<RayCollision> list;
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

                Actor *actor = (Actor *)body.GetUserData();
                if (actor && actor->hasCollisionChannel(channelId))
                {
                    RayCollision collision({v1, (Actor *)body.GetUserData()});
                    list.push_back(collision);
                    lock.ReleaseLock();
                }
            }
    }
    return list;
}

std::list<Actor *> *LayerActors::getActorsList()
{
    return &actors;
}

std::list<Actor *> LayerActors::getActorsByName(std::string name)
{
    std::list<Actor *> list;

    if (actors.size() > 0)
    {
        for (auto actor = actors.begin(); actor != actors.end(); ++actor)
        {
            if (*(*actor)->getActorName() == name)
            {
                list.push_back(*actor);
            }
        }
    }

    return list;
}

std::list<Actor *> LayerActors::getActorsByPartName(std::string partOfName)
{
    std::list<Actor *> list;

    if (actors.size() > 0)
    {
        for (auto actor = actors.begin(); actor != actors.end(); ++actor)
        {
            if ((*actor)->getActorName()->find(name) != std::string::npos)
            {
                list.push_back(*actor);
            }
        }
    }

    return list;
}

LayerActors *LayerActors::getActorsLayer(Actor *actor)
{
    return (LayerActors *)actor->getCurrentLayer();
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

void LayerActors::setVisible(bool state)
{
    bIsVisible = state;
}

bool LayerActors::isVisible()
{
    return bIsVisible;
}

void LayerActors::setProcessingEnabled(bool state)
{
    bProcessingEnabled = state;
}
bool LayerActors::isProcessingEnabled()
{
    return bProcessingEnabled;
}

Camera *LayerActors::getActiveCamera()
{
    return activeCamera;
}

void LayerActors::setAmbientColor(float r, float g, float b)
{
    ambientColor[0] = r;
    ambientColor[1] = g;
    ambientColor[2] = b;
}