#include "stage.h"
#include <math.h>
#include "glew.h"
#include "wglew.h"
#include "SDL.h"

ShadersManager *Stage::shadersManager = nullptr;

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

    auto screenShader = shadersManager->getScreenShader();
    shadersManager->switchShader(screenShader);
    glBindTexture(GL_TEXTURE_2D, view->getTexture());
    glBindVertexArray(screenShader->vao);
    glDrawArrays(GL_QUADS, 0, 4);

    view->swapBuffers();
}

void Stage::setClearColor(unsigned char r, unsigned char g, unsigned char b)
{
    clearColor[0] = (float)r / 255.0f;
    clearColor[1] = (float)g / 255.0f;
    clearColor[2] = (float)b / 255.0f;
}

void Stage::setShadersManager(ShadersManager *shadersManager)
{
    Stage::shadersManager = shadersManager;
}

void Stage::sortLayers()
{
}

void Stage::clearTarget()
{
    auto clearShader = shadersManager->getClearShader();
    shadersManager->switchShader(clearShader);
    glUniform3fv(clearShader->v3ColorLoc, 1, clearColor);
    glBindVertexArray(clearShader->vao);
    glDrawArrays(GL_QUADS, 0, 4);
}