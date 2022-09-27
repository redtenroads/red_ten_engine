// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/component.h"

Component::Component()
{
}
Component::~Component()
{
}

void Component::prepare()
{
}

void Component::process(float delta)
{
}

void Component::render(Matrix4 &vpMatrix, Transformation *tf)
{
}

void Component::renderLightPhase(Matrix4 &vpMatrix, Transformation *tf)
{
}

PhysicsEntitySphere *Component::addPhysics2dCircle(float radius)
{
    return addPhysics2dCircle(radius, 0, 0, 0);
}

PhysicsEntitySphere *Component::addPhysics2dCircle(float radius, float px, float py, float pz)
{
    auto newPhysicsEntity = new PhysicsEntitySphere(radius * SIZE_MULTIPLIER, px, py, pz);
    physicsEntities.push_back(newPhysicsEntity);
    return newPhysicsEntity;
}

PhysicsEntityBox *Component::addPhysics2dBox(float width, float height)
{
    return addPhysics2dBox(width, height, 0, 0, 0);
}

PhysicsEntityBox *Component::addPhysics2dBox(float width, float height, float px, float py, float pz)
{
    auto newPhysicsEntity = new PhysicsEntityBox(width * SIZE_MULTIPLIER, height * SIZE_MULTIPLIER, px, py, pz);
    physicsEntities.push_back(newPhysicsEntity);
    return newPhysicsEntity;
}

Matrix4 Component::getLocalspaceMatrix()
{
    return Matrix4(1.0f);
}
