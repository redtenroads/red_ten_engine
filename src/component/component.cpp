// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/component.h"
#include "math/glm/gtc/type_ptr.hpp"
#include "common/commonShaders.h"
#include "opengl/glew.h"
#include <math.h>

Component::Component()
{
}
Component::~Component()
{
}

void Component::prepare(Entity *owner)
{
    this->owner = owner;
}

void Component::process(float delta)
{
}

void Component::render(Matrix4 &vpMatrix, Transformation *tf)
{
    bool state = onRenderPrepare(vpMatrix, tf, false);
    if (state)
    {
        int drawAmount = getVertexAmount();
        if (drawAmount)
            glDrawArrays(drawAmount == 4 ? GL_QUADS : GL_TRIANGLES, 0, drawAmount);
    }
}

void Component::shadowRender(Matrix4 &vpMatrix, Transformation *tf)
{
    bool state = onRenderPrepare(vpMatrix, tf, true);
    if (state)
    {
        int drawAmount = getVertexAmount();
        if (drawAmount)
            glDrawArrays(GL_TRIANGLES, 0, drawAmount);
    }
}

void Component::renderLightPhase(Matrix4 &vpMatrix, unsigned int shadowMapTexture)
{
}

Matrix4 Component::preparePreShadowPhase(Vector3 cameraPosition)
{
    Matrix4 m;
    return m;
}

bool Component::onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage)
{
    return false;
}

void Component::prepareColorMode()
{
    switch (colorMode)
    {
    case ComponentColorMode::Lit:
        return glBlendFunc(GL_ZERO, GL_ONE);
    case ComponentColorMode::Alpha:
        return glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    case ComponentColorMode::Addition:
        return glBlendFunc(GL_ONE, GL_ONE);
    }
}

int Component::getVertexAmount()
{
    return 0;
}

PhysicsEntitySphere *Component::addPhysics2dCircle(float radius)
{
    return addPhysics2dCircle(radius, 0, 0, 0);
}

PhysicsEntitySphere *Component::addPhysics2dCircle(float radius, float px, float py, float pz)
{
    auto newPhysicsEntity = new PhysicsEntitySphere(radius * SIZE_MULTIPLIER, px, py, pz);
    physicsEntities.push_back(newPhysicsEntity);
    if (owner)
        owner->childUpdated();
    return newPhysicsEntity;
}

PhysicsEntitySphere *Component::addPhysicsSphere(float radius)
{
    return addPhysicsSphere(radius, 0, 0, 0);
}

PhysicsEntitySphere *Component::addPhysicsSphere(float radius, float px, float py, float pz)
{
    auto newPhysicsEntity = new PhysicsEntitySphere(radius * SIZE_MULTIPLIER, px, py, pz);
    physicsEntities.push_back(newPhysicsEntity);
    if (owner)
        owner->childUpdated();
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
    if (owner)
        owner->childUpdated();
    return newPhysicsEntity;
}

PhysicsEntityBox *Component::addPhysics3dBox(float width, float height, float depth)
{
    return addPhysics3dBox(width, height, depth, 0, 0, 0);
}

PhysicsEntityBox *Component::addPhysics3dBox(float width, float height, float depth, float px, float py, float pz)
{
    auto newPhysicsEntity = new PhysicsEntityBox(width * SIZE_MULTIPLIER, height * SIZE_MULTIPLIER, depth * SIZE_MULTIPLIER, px, py, pz);
    physicsEntities.push_back(newPhysicsEntity);
    if (owner)
        owner->childUpdated();
    return newPhysicsEntity;
}

PhysicsEntityGeometry *Component::addPhysicsGeometry(Geometry *geometry)
{
    auto newPhysicsEntity = new PhysicsEntityGeometry(geometry);
    physicsEntities.push_back(newPhysicsEntity);
    if (owner)
        owner->childUpdated();
    return newPhysicsEntity;
}

Matrix4 Component::getLocalspaceMatrix()
{
    return Matrix4(1.0f);
}
