// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "shaders/shader.h"
#include "shaders/rawShader.h"
#include "controller/resourceController.h"
#include "shaders/lightningShader.h"

class CommonShaders
{
public:
    static void build();

    static Shader *shader;
    static ResourceController *resourceController;

    static Mesh *spriteMesh;
    static Mesh *screenMesh;

    static Shader *spriteShader;
    static Shader *spriteFrameShader;
    static Shader *screenShader;
    static RawShader *effectShader;
    static RawShader *initialLightningShader;

    static LightningShader *sunShader;
    static LightningShader *omniShader;
};
