#pragma once
#include "utils.h"
#include "view.h"
#include "physicsManager.h"
#include "shadersManager.h"

class Layer
{
protected:
    Layer(std::string name, int index);
    int index = 0;
    std::string name;

    static PhysicsManager *physicsManager;
    static ShadersManager *shadersManager;

public:
    virtual void process(float delta);
    virtual void render(View *view);

    static void setPhysicsManager(PhysicsManager *physicsManager);
    static void setShadersManager(ShadersManager *shadersManager);
};
