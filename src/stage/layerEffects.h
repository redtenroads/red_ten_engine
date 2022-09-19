// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "shaders/effect.h"
#include "stage/layer.h"
#include <vector>

class LayerEffects : public Layer
{
public:
    LayerEffects(std::string name, int index);
    EXPORT void addEffect(Effect *effect);
    EXPORT void removeEffect(Effect *effect);
    EXPORT void clearEffects();
    EXPORT void render(View *view);

protected:
    std::vector<Effect *> effects;
    unsigned int framebuffer = 0;
    unsigned int renderedTexture = 0;
};
