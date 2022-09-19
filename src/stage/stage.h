// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "stage/layer.h"
#include "stage/layerActors.h"
#include "stage/layerEffects.h"
#include "controller/shadersController.h"
#include "os/view.h"
#include <list>

class Stage
{
public:
    Stage(std::string name);

    EXPORT LayerActors *createLayerActors(std::string name, int index);
    EXPORT LayerEffects *createLayerEffects(std::string name, int index);

    EXPORT void process(float delta);
    EXPORT void present(View *view);

    EXPORT void setClearColor(unsigned char r, unsigned char g, unsigned char b);

    static void setShadersController(ShadersController *shadersController);

protected:
    void clearTarget();
    float clearColor[3] = {0.1f, 0.1f, 0.1f};

    std::string name;
    std::list<Layer *> layers;
    void sortLayers();

    static ShadersController *shadersController;
};
