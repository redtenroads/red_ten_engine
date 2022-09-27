// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "common/commonShaders.h"
#include "stage/layerEffects.h"
#include "opengl/glew.h"
#include "opengl/wglew.h"

LayerEffects::LayerEffects(std::string name, int index) : Layer(name, index)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void LayerEffects::addEffect(Effect *effect)
{
    effects.push_back(effect);
}

void LayerEffects::removeEffect(Effect *effect)
{
    auto it = effects.begin();
    while (it != effects.end())
        if ((*it) == effect)
            it = effects.erase(it);
        else
            ++it;
}

void LayerEffects::clearEffects()
{
    effects.clear();
}

void LayerEffects::render(View *view)
{
    if (effects.size() > 0)
    {
        int width = view->getWidth();
        int height = view->getHeight();
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        RawShader *effectShader = CommonShaders::effectShader;
        Matrix4 m;

        for (auto it = effects.begin(); it != effects.end(); it++)
        {
            auto effect = *it;

            if (effect->isReady() && effect->isEnabled() && effect->getOpacity() > 0.0f)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                glViewport(0, 0, width, height);

                effect->use(m, m);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, view->getTexture());
                CommonShaders::screenMesh->use();
                glDrawArrays(GL_QUADS, 0, 4);

                view->useFrameBuffer();
                effectShader->use(m, m);
                effectShader->setOpacity(effect->getOpacity());

                glBindTexture(GL_TEXTURE_2D, renderedTexture);
                CommonShaders::screenMesh->use();
                glDrawArrays(GL_QUADS, 0, 4);
            }
        }
    }
}