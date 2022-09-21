#pragma once
#include "resource/shader.h"
#include "controller/resourceController.h"

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
    static Shader *effectShader;
    static Shader *clearShader;
};
