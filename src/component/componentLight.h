// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "component/component.h"
#include "shaders/shader.h"
#include "resource/mesh.h"

enum LightType
{
    None = 0,
    Sun,
    Omni
};

class ComponentLight : public Component
{
public:
    EXPORT ComponentLight();
    EXPORT void setupSunLight(Vector3 normal, Vector3 color, bool bCastShadows = false);
    EXPORT void setupOmniLight(float affectDistance, Vector3 color, bool bCastShadows = false);

    EXPORT void renderLightPhase(Matrix4 &vpMatrix, Transformation *tf);

protected:
    float normal[3], color[3];
    float affectDistance = 0.0f;
    bool bCastShadows;
    LightType type = LightType::None;
};
