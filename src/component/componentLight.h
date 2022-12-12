// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "component/component.h"
#include "shaders/shader.h"
#include "resource/resourceMesh.h"

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
    EXPORT void setupSunLight(Vector3 position, Vector3 color, bool bCastShadows = false);
    EXPORT void setupOmniLight(float affectDistance, Vector3 color, bool bCastShadows = false);

    EXPORT bool onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage);

    EXPORT Matrix4 preparePreShadowPhase(Vector3 cameraPosition);
    EXPORT void renderLightPhase(Matrix4 &vpMatrix, unsigned int shadowMapTexture);

    EXPORT void enableShadows();
    EXPORT void disableShadows();

protected:
    float normal[3], color[3];
    float affectDistance = 0.0f;
    LightType type = LightType::None;
    Matrix4 m1;
    Matrix4 lightViewProjection;

    Transformation *tf = nullptr;
};
