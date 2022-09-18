#pragma once
#include "utils.h"
#include "matrix4.h"
#include "transformation.h"
#include "shadersManager.h"
#include "physicsEntity.h"
#include "physicsEntityBox.h"
#include "physicsEntitySphere.h"
#include <list>

class Component
{
public:
    EXPORT Component();
    EXPORT virtual ~Component();
    EXPORT virtual void prepare();
    EXPORT virtual void process(float delta);
    EXPORT virtual void render(Matrix4 &vpMatrix, Transformation *tf);
    EXPORT virtual void destroy();
    EXPORT bool isMarkedForDestroy();

    EXPORT PhysicsEntitySphere *addPhysics2dCircle(float radius);
    EXPORT PhysicsEntitySphere *addPhysics2dCircle(float radius, float px, float py, float pz);
    EXPORT PhysicsEntityBox *addPhysics2dBox(float width, float height);
    EXPORT PhysicsEntityBox *addPhysics2dBox(float width, float height, float px, float py, float pz);

    EXPORT virtual Matrix4 getLocalspaceMatrix();

    static void setShadersManager(ShadersManager *shadersManager);

    std::list<PhysicsEntity *> physicsEntities;
    
    Transformation transform;
protected:
    bool bMarkedForDestroy = false;
    static ShadersManager *shadersManager;
};
