// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "stage/layer.h"
#include "stage/layerActors.h"
#include "stage/layerEffects.h"
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

    EXPORT void setClearColor(float r, float g, float b)
    {
        clearColor[0] = r;
        clearColor[1] = g;
        clearColor[2] = b;
    }

protected:
    void sortLayers();

    Renderer *renderer = nullptr;
    std::string name;
    std::list<Layer *> layers;

    float clearColor[3] = {0.15f, 0.15f, 0.15f};
};
