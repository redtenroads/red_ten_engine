#include "layer.h"

PhysicsManager *Layer::physicsManager = nullptr;
ShadersManager *Layer::shadersManager = nullptr;

Layer::Layer(std::string name, int index)
{
    this->name = name;
    this->index = index;
}

void Layer::process(float delta)
{
}

void Layer::render(View *view)
{
}

void Layer::setPhysicsManager(PhysicsManager *physicsManager)
{
    Layer::physicsManager = physicsManager;
}

void Layer::setShadersManager(ShadersManager *shadersManager)
{
    Layer::shadersManager = shadersManager;
}