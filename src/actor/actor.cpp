// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "actor/actor.h"
#include "stage/layerActors.h"

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
#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Constraints/SixDOFConstraint.h>
#include <Jolt/Physics/Constraints/FixedConstraint.h>
#include <Jolt/Physics/Collision/GroupFilterTable.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

// All Jolt symbols are in the JPH namespace
using namespace JPH;

// We're also using STL classes in this example
using namespace std;

PhysicsController *Actor::physicsController = nullptr;

Actor::Actor()
{
    registerClassName("Actor");
}

Actor::~Actor()
{
    removeComponents();
    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        auto body = (Body *)physicsRoot;
        body->SetUserData(0);
        physicsSystem->GetBodyInterface().RemoveBody(body->GetID());
        delete body;
    }
}

void Actor::setActorName(std::string name)
{
    this->name = name;
}

const std::string *Actor::getActorName()
{
    return &this->name;
}

void Actor::setPhysicsMotionType(MotionType mType)
{
    this->mType = mType;
}

void Actor::markPhysicsUpdateNeeded()
{
    bPhysicsNeedsToBeRebuild = true;
}

std::list<Actor *> *Actor::getLayerActorsList()
{
    if (layer)
        return ((LayerActors *)layer)->getActorsList();
    return nullptr;
}

void Actor::removeComponents()
{
    for (auto it = components.begin(); it != components.end(); it++)
    {
        delete (*it);
    }
    components.clear();
    bPhysicsNeedsToBeRebuild = true;
}

void Actor::prepareNewComponent(Component *component)
{
    this->components.push_back(component);
    component->prepare(this);
    bPhysicsNeedsToBeRebuild = true;
}

void Actor::providePhysicsSystem(PhysicsDescriptor *system)
{
    this->system = system;
    bPhysicsNeedsToBeRebuild = true;
}

void Actor::setZAxisLocked(bool state)
{
    bIsZAxisLocked = state;
    bPhysicsNeedsToBeRebuild = true;
}

void Actor::setZAxisRotationLocked(bool state)
{
    bIsZRotationLocked = state;
    bPhysicsNeedsToBeRebuild = true;
}

void Actor::preSyncPhysics()
{
    if (bPhysicsNeedsToBeRebuild)
        updatePhysics();

    if (system && physicsRoot && transform.isDirty())
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();

        auto position = transform.getPosition();
        auto rotation = transform.getRotation();
        bodyInterface.SetPositionAndRotation(
            rootId,
            Vec3(position.x * SIZE_MULTIPLIER, position.y * SIZE_MULTIPLIER, (bIsZAxisLocked ? zLockedPosition : position.z) * SIZE_MULTIPLIER),
            Quat::sEulerAngles(Vec3(rotation.x, rotation.y, rotation.z)),
            EActivation::DontActivate);

        transform.update();
    }
}

void Actor::afterSyncPhysics()
{
    if (system && physicsRoot && mType != MotionType::Static)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();

        Vec3 position = bodyInterface.GetPosition(rootId);
        Quat rotationQuat = bodyInterface.GetRotation(rootId);
        Vec3 r = rotationQuat.GetEulerAngles();

        transform.setPosition(position.GetX() / SIZE_MULTIPLIER, position.GetY() / SIZE_MULTIPLIER, bIsZAxisLocked ? zLockedPosition : position.GetZ() / SIZE_MULTIPLIER);
        transform.setRotation(bIsZAxisLocked ? 0.0f : r.GetX(), bIsZAxisLocked ? 0.0f : r.GetY(), r.GetZ());
        transform.update();
    }
}

void Actor::process(float delta)
{
    onProcess(delta);
    if (components.size() > 0)
    {
        auto it = components.begin();
        while (it != components.end())
        {
            (*it)->process(delta);
            if ((*it)->isDestroyed())
            {
                delete (*it);
                it = components.erase(it);
                bPhysicsNeedsToBeRebuild = true;
            }
            else
                ++it;
        }
    }
}

void Actor::setRestitution(float newValue)
{
    this->restitution = newValue;
    bPhysicsNeedsToBeRebuild = true;
}

float Actor::getRestitution()
{
    return restitution;
}

void Actor::setFriction(float newValue)
{
    this->friction = newValue;
    bPhysicsNeedsToBeRebuild = true;
}

float Actor::getFriction()
{
    return friction;
}

void Actor::setFrictionAndRestitution(float newFrictionValue, float newRestitutionValue)
{
    this->friction = newFrictionValue;
    this->restitution = newRestitutionValue;
    bPhysicsNeedsToBeRebuild = true;
}

Vector3 Actor::getLinearVelocity()
{
    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();
        auto velocity = bodyInterface.GetLinearVelocity(rootId);
        return Vector3(velocity.GetX(), velocity.GetY(), velocity.GetZ());
    }
    return Vector3(0, 0, 0);
}

void Actor::setLinearVelocity(Vector3 v)
{
    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();
        bodyInterface.SetLinearVelocity(rootId, Vec3(v.x, v.y, v.z));
    }
}

void Actor::addLinearVelocity(Vector3 v)
{
    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();
        bodyInterface.AddLinearVelocity(rootId, Vec3(v.x, v.y, v.z));
    }
}

Vector3 Actor::getAngularVelocity()
{
    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();
        auto velocity = bodyInterface.GetAngularVelocity(rootId);
        return Vector3(velocity.GetX(), velocity.GetY(), velocity.GetZ());
    }
    return Vector3(0, 0, 0);
}

void Actor::setAngularVelocity(Vector3 v)
{
    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();
        bodyInterface.SetAngularVelocity(rootId, Vec3(v.x, v.y, v.z));
    }
}

void Actor::addAngularVelocity(Vector3 v)
{
    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();
        bodyInterface.AddLinearAndAngularVelocity(rootId, Vec3(0.0f, 0.0f, 0.0f), Vec3(v.x, v.y, v.z));
    }
}

void Actor::AddLinearAndAngularVelocity(Vector3 vv, Vector3 av)
{
    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
        BodyID rootId = ((Body *)physicsRoot)->GetID();
        bodyInterface.AddLinearAndAngularVelocity(rootId, Vec3(vv.x, vv.y, vv.z), Vec3(av.x, av.y, av.z));
    }
}

void Actor::onSpawned()
{
    updatePhysics();
}

void Actor::onProcess(float delta)
{
}

void Actor::onRender(Matrix4 &vpMatrix, std::vector<Component *> *lights)
{
    bHasBlended = false;
    if (components.size() > 0)
        for (auto it = components.begin(); it != components.end(); it++)
            if ((*it)->isVisible())
            {
                if ((*it)->isUsingBlendingPhase())
                    bHasBlended = true;
                else
                    (*it)->render(vpMatrix, &transform);

                if ((*it)->isUsingLightPhase())
                {
                    lights->push_back((*it));
                }
            }
}

void Actor::onRenderShadowed(Matrix4 &vpMatrix)
{
    if (components.size() > 0)
        for (auto it = components.begin(); it != components.end(); it++)
            if ((*it)->isVisible() && !(*it)->isUsingBlendingPhase())
                (*it)->shadowRender(vpMatrix, &transform);
}

void Actor::onRenderBlended(Matrix4 &vpMatrix)
{
    if (components.size() > 0)
        for (auto it = components.begin(); it != components.end(); it++)
            if ((*it)->isVisible() && (*it)->isUsingBlendingPhase())
                (*it)->render(vpMatrix, &transform);
}

void Actor::onDestroy()
{
    Destroyable::onDestroy();

    if (components.size() > 0)
        for (auto it = components.begin(); it != components.end(); it++)
            (*it)->destroy();
}

void Actor::onCollide(Actor *hitWith, Vector3 v)
{
}

void Actor::onCollidePersisted(Actor *hitWith, Vector3 v)
{
}

void Actor::assignCollisionChannel(int channelId)
{
    if (!hasCollisionChannel(channelId))
    {
        collisionChannels.push_back(channelId);
    }
}

void Actor::removeCollisionChannel(int channelId)
{
    for (auto channel = collisionChannels.begin(); channel != collisionChannels.end(); ++channel)
    {
        if (*channel == channelId)
        {
            collisionChannels.erase(channel);
            break;
        }
    }
}

bool Actor::hasCollisionChannel(int channelId)
{
    if (channelId == 0)
        return true;
    for (auto channel = collisionChannels.begin(); channel != collisionChannels.end(); ++channel)
    {
        if (*channel == channelId)
            return true;
    }
    return false;
}

void Actor::childUpdated()
{
    bPhysicsNeedsToBeRebuild = true;
}

void Actor::setPhysicsController(PhysicsController *physicsController)
{
    Actor::physicsController = physicsController;
}

void Actor::updatePhysics()
{
    bPhysicsNeedsToBeRebuild = false;

    if (system && physicsRoot)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
        auto body = (Body *)physicsRoot;
        body->SetUserData(0);
        physicsSystem->GetBodyInterface().RemoveBody(body->GetID());
        physicsRoot = nullptr;
    }

    if (system && components.size() > 0)
    {
        int collisionsAmount = 0;
        for (auto component = components.begin(); component != components.end(); component++)
            collisionsAmount += (*component)->physicsEntities.size();

        if (collisionsAmount > 0)
        {
            auto position = transform.getPosition();
            auto rotation = transform.getRotation();
            auto scale = transform.getScale();

            PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;
            BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
            StaticCompoundShapeSettings *compoundRoot = new StaticCompoundShapeSettings;
            int shapesAdded = 0;

            for (auto component = components.begin(); component != components.end(); component++)
            {
                auto pEntities = &(*component)->physicsEntities;
                if (pEntities->size() > 0)
                {
                    auto transform = &(*component)->transform;
                    auto relativePosition = transform->getPosition();
                    auto relativeRotation = transform->getRotation();
                    for (auto physicsEntitie = pEntities->begin(); physicsEntitie != pEntities->end(); physicsEntitie++)
                    {
                        ShapeSettings *shape = (ShapeSettings *)(*physicsEntitie)->getShape(scale);

                        if (shape)
                        {
                            shapesAdded++;
                            compoundRoot->AddShape(
                                Vec3(
                                    (relativePosition.x + (*physicsEntitie)->x) * SIZE_MULTIPLIER * scale.x,
                                    (relativePosition.y + (*physicsEntitie)->y) * SIZE_MULTIPLIER * scale.y,
                                    (relativePosition.z + (*physicsEntitie)->z) * SIZE_MULTIPLIER * scale.z),
                                Quat(relativeRotation.x, relativeRotation.y, relativeRotation.z, 1.0f),
                                shape);
                        }
                    }
                }
            }

            if (shapesAdded == 0)
            {
                delete compoundRoot;
                return;
            }

            Body *body = bodyInterface.CreateBody(
                BodyCreationSettings(compoundRoot,
                                     Vec3(position.x * SIZE_MULTIPLIER, position.y * SIZE_MULTIPLIER, position.z * SIZE_MULTIPLIER),
                                     Quat(rotation.x, rotation.y, rotation.z, 1.0f),
                                     (EMotionType)mType,
                                     physicsController->getMovingLayerId()));

            bodyInterface.AddBody(body->GetID(), EActivation::Activate);
            bodyInterface.SetRestitution(body->GetID(), restitution);
            bodyInterface.SetFriction(body->GetID(), friction);
            body->SetUserData((unsigned long long)this);
            processPhysicsContraints(body);
            physicsSystem->OptimizeBroadPhase();
            physicsRoot = body;
        }
    }
}

void Actor::processPhysicsContraints(void *body)
{
    if (bIsZAxisLocked || bIsZRotationLocked)
    {
        PhysicsSystem *physicsSystem = (PhysicsSystem *)system->system;

        SixDOFConstraintSettings constraintSettings;
        if (bIsZAxisLocked)
        {
            constraintSettings.MakeFixedAxis(SixDOFConstraintSettings::EAxis::TranslationZ);
            constraintSettings.MakeFixedAxis(SixDOFConstraintSettings::EAxis::RotationX);
            constraintSettings.MakeFixedAxis(SixDOFConstraintSettings::EAxis::RotationY);
        }
        if (bIsZRotationLocked)
        {
            constraintSettings.MakeFixedAxis(SixDOFConstraintSettings::EAxis::RotationZ);
        }

        auto constraint = constraintSettings.Create(Body::sFixedToWorld, *(Body *)body);
        physicsSystem->AddConstraint(constraint);
    }
}