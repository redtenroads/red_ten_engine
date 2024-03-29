// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "actor/watchable.h"
#include "common/utils.h"
#include "common/destroyable.h"
#include "common/withDebug.h"
#include "common/withLogger.h"
#include "common/entity.h"
#include "math/math.h"
#include "math/transformation.h"
#include "component/component.h"
#include "controller/physicsController.h"
#include "physics/physicsEntity.h"
#include <string>
#include <list>
#include <vector>

class Actor : public Entity, public Watchable<Actor>, public Destroyable, public WithDebug, public WithLogger
{
public:
    EXPORT Actor();
    EXPORT virtual ~Actor();

    EXPORT void setActorName(std::string name);
    EXPORT const std::string *getActorName();

    EXPORT void setPhysicsMotionType(MotionType mType);
    EXPORT void markPhysicsUpdateNeeded();
    EXPORT std::list<Actor *> *getLayerActorsList();

    template <class T, typename std::enable_if<std::is_base_of<Component, T>::value>::type * = nullptr>
    EXPORT T *createComponent()
    {
        auto newComonent = new T();
        prepareNewComponent(newComonent);
        return newComonent;
    }
    EXPORT void removeComponents();
    EXPORT void prepareNewComponent(Component *component);
    EXPORT void providePhysicsSystem(PhysicsDescriptor *system);
    EXPORT void setZAxisLocked(bool state);
    EXPORT void setZAxisRotationLocked(bool state);

    EXPORT virtual void preSyncPhysics();
    EXPORT virtual void afterSyncPhysics();

    EXPORT void process(float delta);

    EXPORT void setRestitution(float newValue);
    EXPORT float getRestitution();
    EXPORT void setFriction(float newValue);
    EXPORT float getFriction();
    EXPORT void setFrictionAndRestitution(float newFrictionValue, float newRestitutionValue);

    EXPORT Vector3 getLinearVelocity();
    EXPORT void setLinearVelocity(Vector3 v);
    EXPORT void addLinearVelocity(Vector3 v);

    EXPORT Vector3 getAngularVelocity();
    EXPORT void setAngularVelocity(Vector3 v);
    EXPORT void addAngularVelocity(Vector3 v);
    EXPORT void AddLinearAndAngularVelocity(Vector3 vv, Vector3 av);

    EXPORT virtual void onSpawned();
    EXPORT virtual void onProcess(float delta);
    EXPORT virtual void onRender(Matrix4 &vpMatrix, std::vector<Component *> *lights);
    EXPORT virtual void onRenderShadowed(Matrix4 &vpMatrix);
    EXPORT virtual void onRenderBlended(Matrix4 &vpMatrix);
    EXPORT virtual void onDestroy();
    EXPORT virtual void onCollide(Actor *hitWith, Vector3 v);
    EXPORT virtual void onCollidePersisted(Actor *hitWith, Vector3 v);

    EXPORT bool hasBlended() { return bHasBlended; };

    EXPORT void assignCollisionChannel(int channelId);
    EXPORT void removeCollisionChannel(int channelId);
    EXPORT bool hasCollisionChannel(int channelId);

    EXPORT void childUpdated();

    static void setPhysicsController(PhysicsController *physicsController);

protected:
    void updatePhysics();
    void processPhysicsContraints(void *body);

    bool bIsZAxisLocked = false;
    bool bIsZRotationLocked = false;
    bool bIsVisible = true;
    bool bHasBlended = false;
    bool bPhysicsNeedsToBeRebuild = false;

    MotionType mType = MotionType::Static;

    PhysicsDescriptor *system = nullptr;
    void *physicsRoot = nullptr;

    float restitution = 0.5f;
    float friction = 1.0f;
    float zLockedPosition = 0.0f;

    std::list<Component *> components;
    std::vector<int> collisionChannels;

    static PhysicsController *physicsController;
    std::string name = "actor";
};
