// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/entity.h"
#include "math/math.h"
#include "math/transformation.h"
#include "physics/physicsEntity.h"
#include "physics/physicsEntityBox.h"
#include "physics/physicsEntitySphere.h"
#include "physics/physicsEntityGeometry.h"
#include "common/destroyable.h"
#include "common/renderer.h"
#include <list>

enum class ComponentColorMode
{
    Lit = 0,
    Alpha = 1,
    Addition = 2,
};

class Component : public Destroyable
{
public:
    EXPORT Component();
    EXPORT virtual ~Component();
    EXPORT void prepare(Entity *owner);
    EXPORT virtual void process(float delta);

    EXPORT void render(Matrix4 &vpMatrix, Transformation *tf);
    EXPORT void shadowRender(Matrix4 &vpMatrix, Transformation *tf);
    EXPORT virtual void renderLightPhase(Matrix4 &vpMatrix, unsigned int shadowMapTexture);
    EXPORT virtual Matrix4 preparePreShadowPhase(Vector3 cameraPosition);

    EXPORT virtual bool onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage);
    EXPORT void prepareColorMode();
    EXPORT virtual int getVertexAmount();

    EXPORT PhysicsEntitySphere *addPhysics2dCircle(float radius);
    EXPORT PhysicsEntitySphere *addPhysics2dCircle(float radius, float px, float py, float pz);
    EXPORT PhysicsEntitySphere *addPhysicsSphere(float radius);
    EXPORT PhysicsEntitySphere *addPhysicsSphere(float radius, float px, float py, float pz);
    EXPORT PhysicsEntityBox *addPhysics2dBox(float width, float height);
    EXPORT PhysicsEntityBox *addPhysics2dBox(float width, float height, float px, float py, float pz);
    EXPORT PhysicsEntityBox *addPhysics3dBox(float width, float height, float depth);
    EXPORT PhysicsEntityBox *addPhysics3dBox(float width, float height, float depth, float px, float py, float pz);

    EXPORT PhysicsEntityGeometry *addPhysicsGeometry(Geometry *geometry);

    EXPORT virtual Matrix4 getLocalspaceMatrix();

    EXPORT inline bool isUsingBlendingPhase() { return colorMode != ComponentColorMode::Lit; }
    EXPORT inline bool isUsingLightPhase() { return bUseLightPhase; }
    EXPORT inline bool isUsingShadowPhase() { return bUseShadowPhase; }

    EXPORT inline void setVisibility(bool state) { bIsVisible = state; }
    EXPORT inline bool isVisible() { return bIsVisible; }

    std::list<PhysicsEntity *> physicsEntities;
    Transformation transform;

    ComponentColorMode colorMode = ComponentColorMode::Lit;
protected:
    bool bUseLightPhase = false;
    bool bUseShadowPhase = false;
    bool bIsVisible = true;

    Entity *owner = nullptr;
};
