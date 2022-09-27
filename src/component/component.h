// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "math/math.h"
#include "math/transformation.h"
#include "physics/physicsEntity.h"
#include "physics/physicsEntityBox.h"
#include "physics/physicsEntitySphere.h"
#include "common/destroyable.h"
#include "common/renderer.h"
#include <list>

class Component : public Destroyable
{
public:
    EXPORT Component();
    EXPORT virtual ~Component();
    EXPORT virtual void prepare();
    EXPORT virtual void process(float delta);
    EXPORT virtual void render(Matrix4 &vpMatrix, Transformation *tf);
    EXPORT virtual void renderLightPhase(Matrix4 &vpMatrix, Transformation *tf);

    EXPORT PhysicsEntitySphere *addPhysics2dCircle(float radius);
    EXPORT PhysicsEntitySphere *addPhysics2dCircle(float radius, float px, float py, float pz);
    EXPORT PhysicsEntityBox *addPhysics2dBox(float width, float height);
    EXPORT PhysicsEntityBox *addPhysics2dBox(float width, float height, float px, float py, float pz);

    EXPORT virtual Matrix4 getLocalspaceMatrix();

    EXPORT inline bool isUsingBlendingPhase() { return bUseBlendingPhase; }
    EXPORT inline bool isUsingLightPhase() { return bUseLightPhase; }

    EXPORT inline void setVisibility(bool state) { bIsVisible = state; }
    EXPORT inline bool isVisible() { return bIsVisible; }

    std::list<PhysicsEntity *> physicsEntities;
    Transformation transform;

protected:
    bool bUseBlendingPhase = false;
    bool bUseLightPhase = false;
    bool bIsVisible = true;
};
