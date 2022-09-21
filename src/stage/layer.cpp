// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "stage/layer.h"

PhysicsController *Layer::physicsController = nullptr;

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

void Layer::setPhysicsController(PhysicsController *physicsController)
{
    Layer::physicsController = physicsController;
}
