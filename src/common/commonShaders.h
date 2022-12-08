// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "shaders/shader.h"
#include "shaders/rawShader.h"
#include "controller/resourceController.h"
#include "shaders/lightningShader.h"
#include "common/withLogger.h"

class CommonShaders : public WithLogger
{
public:
    static void build();

    static Mesh *spriteMesh;
    static Mesh *screenMesh;

    static Shader *spriteShader;
    static Shader *spriteFrameShader;
    static Shader *screenShader;
    static RawShader *effectShader;
    static RawShader *initialLightningShader;

    static LightningShader *sunShader;
    static LightningShader *sunWithShadowShader;
    static LightningShader *omniShader;

    static ResourceController *resourceController;

    EXPORT static Mesh *getSpriteMesh();
};
