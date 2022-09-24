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
    view->prepare();
    clearTarget();

    for (auto it = layers.begin(); it != layers.end(); ++it)
    {
        (*it)->render(view);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    auto screenShader = CommonShaders::screenShader;
    screenShader->use();

    glBindTexture(GL_TEXTURE_2D, view->getTexture());
    CommonShaders::screenMesh->use();
    glDrawArrays(GL_QUADS, 0, 4);

    view->swapBuffers();
}

void Stage::setClearColor(unsigned char r, unsigned char g, unsigned char b)
{
    clearColor[0] = (float)r / 255.0f;
    clearColor[1] = (float)g / 255.0f;
    clearColor[2] = (float)b / 255.0f;
}

void Stage::sortLayers()
{
}

void Stage::clearTarget()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    auto clearShader = CommonShaders::clearShader;
    clearShader->use();
    glUniform3fv(clearShader->v3ColorLoc, 1, clearColor);
    CommonShaders::screenMesh->use();
    glDrawArrays(GL_QUADS, 0, 4);
}