#include "common/commonShaders.h"
#include <stdio.h>

extern float spritePoints[];
extern float screenPoints[];

extern const char *spriteVertexShader;
extern const char *spriteFramedVertexShader;
extern const char *spriteFragmentShader;

extern const char *screenVertexShader;
extern const char *screenFragmentShader;

extern const char *clearFragmentShader;

extern const char *meshVertexShader;
extern const char *meshFragmentShader;

Shader *CommonShaders::shader = nullptr;
ResourceController *CommonShaders::resourceController = nullptr;

Mesh *CommonShaders::spriteMesh = nullptr;
Mesh *CommonShaders::screenMesh = nullptr;

Shader *CommonShaders::spriteShader = nullptr;
Shader *CommonShaders::spriteFrameShader = nullptr;
Shader *CommonShaders::screenShader = nullptr;
Shader *CommonShaders::effectShader = nullptr;
Shader *CommonShaders::clearShader = nullptr;

Shader *CommonShaders::meshShader = nullptr;

void CommonShaders::build()
{
    spriteMesh = resourceController->addMesh();
    spriteMesh->setupByArray8f(spritePoints, 8 * 4);

    screenMesh = resourceController->addMesh();
    screenMesh->setupByArray8f(screenPoints, 8 * 4);

    printf("compiling sprite shader ...\n");
    spriteShader = resourceController->addShader(spriteVertexShader, spriteFragmentShader);

    printf("compiling mesh shader ...\n");
    meshShader = resourceController->addShader(meshVertexShader, meshFragmentShader);

    printf("compiling framed sprite shader ...\n");
    spriteFrameShader = resourceController->addShader(spriteFramedVertexShader, spriteFragmentShader);

    printf("compiling screen shader ...\n");
    screenShader = resourceController->addShader(screenVertexShader, screenFragmentShader);

    printf("compiling effect shader ...\n");
    effectShader = resourceController->addShader(screenVertexShader, spriteFragmentShader);

    printf("compiling clear shader ...\n");
    clearShader = resourceController->addShader(screenVertexShader, clearFragmentShader);

    printf("done\n");
}

float spritePoints[] = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

float screenPoints[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};

const char *spriteVertexShader =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormals;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 mTransform;\n"
    "uniform mat4 mViewProjection;\n"
    "void main() {\n"
    "   gl_Position = mViewProjection * mTransform * vec4(aPos, 1.0);\n"
    "   texCoord = aTexCoord;\n"
    "}\n";

const char *spriteFramedVertexShader =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormals;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 mTransform;\n"
    "uniform mat4 mViewProjection;\n"
    "uniform vec2 aTexCoordShift;\n"
    "uniform vec2 aTexCoordMul;\n"
    "void main() {\n"
    "   gl_Position = mViewProjection * mTransform * vec4(aPos, 1.0);\n"
    "   texCoord = vec2(aTexCoord.x * aTexCoordMul.x + aTexCoordShift.x, aTexCoord.y * aTexCoordMul.y + aTexCoordShift.y);\n"
    "}\n";

const char *spriteFragmentShader =
    "#version 400\n"
    "out vec4 fragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D t;\n"
    "uniform float fOpacity;\n"
    "void main() {\n"
    "   vec4 color = texture(t, texCoord);\n"
    "   fragColor = color * fOpacity;\n"
    "}\n";

const char *screenVertexShader =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormals;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   texCoord = aTexCoord;\n"
    "}\n";

const char *screenFragmentShader =
    "#version 400\n"
    "out vec4 fragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D t;\n"
    "void main() {\n"
    "   fragColor = texture(t, texCoord);\n"
    "}\n";

const char *clearFragmentShader =
    "#version 400\n"
    "out vec4 fragColor;\n"
    "uniform vec3 clearColor;\n"
    "void main() {\n"
    "   fragColor = vec4(clearColor, 1.0);\n"
    "}\n";

const char *meshVertexShader =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormals;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 mTransform;\n"
    "uniform mat4 mViewProjection;\n"
    "void main() {\n"
    "   gl_Position = mViewProjection * mTransform * vec4(aPos, 1.0);\n"
    "   texCoord = aTexCoord;\n"
    "}\n";

const char *meshFragmentShader =
    "#version 400\n"
    "out vec4 fragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D t;\n"
    "void main() {\n"
    "   vec4 color = texture(t, texCoord);\n"
    "   fragColor = color;\n"
    "}\n";