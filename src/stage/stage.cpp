// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "common/commonShaders.h"
#include "stage/stage.h"
#include "opengl/glew.h"
#include "opengl/wglew.h"
#include <SDL.h>
#include <math.h>

Stage::Stage(std::string name)
{
    this->name = name;

    debugLayer = new LayerDebug(debugLayerIndex);
    layers.push_back(debugLayer);
    sortLayers();
}

LayerActors *Stage::createLayerActors(std::string name, int index)
{
    auto newLayerActors = new LayerActors(name, index);
    layers.push_back(newLayerActors);
    sortLayers();
    return newLayerActors;
}

LayerEffects *Stage::createLayerEffects(std::string name, int index)
{
    auto newLayerEffects = new LayerEffects(name, index);
    layers.push_back(newLayerEffects);
    sortLayers();
    return newLayerEffects;
}

void Stage::process(float delta)
{
    for (auto it = layers.begin(); it != layers.end(); ++it)
    {
        (*it)->process(delta);
    }
}

void Stage::present(View *view)
{
    view->useFrameBuffer();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4 m;
    for (auto it = layers.begin(); it != layers.end(); ++it)
    {
        (*it)->render(view);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    auto screenShader = CommonShaders::screenShader;
    screenShader->use(m, m);

    glBindTexture(GL_TEXTURE_2D, view->getTexture());

    CommonShaders::screenMesh->use();
    glDrawArrays(GL_QUADS, 0, 4);

    view->swapBuffers();
}

void Stage::sortLayers()
{
    layers.sort([](Layer *layer1, Layer *layer2)
                { return layer1->getIndex() < layer2->getIndex(); });
}
