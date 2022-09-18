#include "layerEffects.h"
#include "glew.h"
#include "wglew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

LayerEffects::LayerEffects(std::string name, int index) : Layer(name, index)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Poor filtering. Needed !
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

        auto effectShader = shadersManager->getEffectShader();

        for (auto it = effects.begin(); it != effects.end(); it++)
        {
            auto effect = *it;
            if (effect->getProgramm() && effect->isEnabled() && effect->getOpacity() > 0.0f)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                glViewport(0, 0, width, height);

                glUseProgram(effect->getProgramm());

                glBindTexture(GL_TEXTURE_2D, view->getTexture());
                glBindVertexArray(effectShader->vao);
                glDrawArrays(GL_QUADS, 0, 4);

                view->prepare();
                glUseProgram(effectShader->programm);
                glUniform1f(effectShader->fOpacityLoc, effect->getOpacity());

                glBindTexture(GL_TEXTURE_2D, renderedTexture);
                glBindVertexArray(effectShader->vao);
                glDrawArrays(GL_QUADS, 0, 4);
            }
        }

        shadersManager->switchShader(nullptr);
    }
}