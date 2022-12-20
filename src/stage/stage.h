// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/color.h"
#include "stage/layer.h"
#include "stage/layerActors.h"
#include "stage/layerEffects.h"
#include "stage/layerDebug.h"
#include "os/view.h"
#include "controller/debugController.h"
#include <list>

class Stage
{
public:
    Stage(std::string name);

    EXPORT LayerActors *createLayerActors(std::string name, int index);
    EXPORT LayerEffects *createLayerEffects(std::string name, int index);

    EXPORT void updateDebugLayer();

    EXPORT void process(float delta);
    EXPORT void present(View *view);

    EXPORT void setClearColor(Color color)
    {
        clearColor = color;
    }

    const int debugLayerIndex = 9999;

protected:
    void sortLayers();

    Renderer *renderer = nullptr;
    std::string name;
    std::list<Layer *> layers;

    Color clearColor{0.15f, 0.15f, 0.15f};

    LayerDebug *debugLayer;
};
