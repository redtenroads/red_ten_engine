#pragma once
#include "utils.h"
#include "layer.h"
#include "layerActors.h"
#include "layerEffects.h"
#include "shadersManager.h"
#include "view.h"
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

    static void setShadersManager(ShadersManager *shadersManager);

protected:
    void clearTarget();
    float clearColor[3] = {0.1f, 0.1f, 0.1f};

    std::string name;
    std::list<Layer *> layers;
    void sortLayers();

    static ShadersManager *shadersManager;
};
