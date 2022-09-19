// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "actor/watchable.h"
#include "common/utils.h"
#include "common/destroyable.h"
#include "math/math.h"
#include "math/transformation.h"
#include "component/component.h"
#include "controller/shadersController.h"
#include "controller/physicsController.h"
#include "physics/physicsEntity.h"
#include <string>
#include <list>
#include <vector>

class Actor : public Watchable<Actor>, public Destroyable
{
public:
    EXPORT Actor();
    EXPORT virtual ~Actor();

    EXPORT void enable2dCollisions(void *world, float collisionMultiplier);

    EXPORT void setPhysicsMotionType(MotionType mType);
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
    EXPORT void updatePhysics();

    EXPORT virtual void preSyncPhysics();
    EXPORT virtual void afterSyncPhysics();

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
    

    EXPORT virtual void onSpawned();
    EXPORT virtual void onProcess(float delta);
    EXPORT virtual void onRender(Matrix4 &vpMatrix);
    EXPORT virtual void onDestroy();
    EXPORT virtual void onCollide(Actor *hitWith, Vector3 v);
    EXPORT virtual void onCollidePersisted(Actor *hitWith, Vector3 v);

    EXPORT bool isVisible();
    EXPORT void setVisible(bool state);

    EXPORT const std::string getName();
    EXPORT const std::string getClass();
    EXPORT bool is(std::string name);
    EXPORT bool implements(std::string name);
    EXPORT void setCurrentLayer(void *layer);

    static void setShadersController(ShadersController *shadersController);
    static void setPhysicsController(PhysicsController *physicsController);

    Transformation transform;

    int zDepth = 0;

protected:
    void processPhysicsContraints(void *body);
    EXPORT void registerName(std::string name);

    bool bIsZAxisLocked = false;
    bool bIsZRotationLocked = false;
    bool bIsVisible = true;

    
    MotionType mType = MotionType::Static;

    PhysicsDescriptor *system = nullptr;
    void *physicsRoot = nullptr;
    void *layer = nullptr;

    float restitution = 0.5f;
    float friction = 1.0f;

    std::list<Component *> components;

    static ShadersController *shadersController;
    static PhysicsController *physicsController;

private:
    std::string className = "";
    std::vector<std::string> classChierarchy;
};
