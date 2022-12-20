// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "os/view.h"
#include "controller/physicsController.h"

class Layer
{
protected:
    Layer(std::string name, int index);
    int index = 0;
    std::string name;

    static PhysicsController *physicsController;

public:
    virtual void process(float delta);
    virtual void render(View *view);

    int getIndex();

    static void setPhysicsController(PhysicsController *physicsController);
};
