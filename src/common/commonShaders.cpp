// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

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

extern const char *meshLightningFragmentCode;
extern const char *initialLightningFragmentCode;

extern const char *sunFragmentCode;
extern const char *omniFragmentCode;

Shader *CommonShaders::shader = nullptr;
ResourceController *CommonShaders::resourceController = nullptr;

Mesh *CommonShaders::spriteMesh = nullptr;
Mesh *CommonShaders::screenMesh = nullptr;

Shader *CommonShaders::spriteShader = nullptr;
Shader *CommonShaders::spriteFrameShader = nullptr;
Shader *CommonShaders::screenShader = nullptr;
RawShader *CommonShaders::effectShader = nullptr;
RawShader *CommonShaders::initialLightningShader = nullptr;

LightningShader *CommonShaders::sunShader = nullptr;
LightningShader *CommonShaders::omniShader = nullptr;

void CommonShaders::build()
{
    spriteMesh = resourceController->addMesh();
    spriteMesh->setupByArray8f(spritePoints, 8 * 4);

    screenMesh = resourceController->addMesh();
    screenMesh->setupByArray8f(screenPoints, 8 * 4);

    printf("compiling sprite shader ...\n");
    spriteShader = new RawShader(spriteVertexShader, spriteFragmentShader);
    spriteShader->build();

    printf("compiling framed sprite shader ...\n");
    spriteFrameShader = new RawShader(spriteFramedVertexShader, spriteFragmentShader);
    spriteFrameShader->build();

    printf("compiling sun shader ...\n");
    sunShader = new LightningShader(screenVertexShader, sunFragmentCode);
    sunShader->build();

    printf("compiling omni shader ...\n");
    omniShader = new LightningShader(screenVertexShader, omniFragmentCode);
    sunShader->build();

    printf("compiling screen shader ...\n");
    screenShader = new RawShader(screenVertexShader, screenFragmentShader);
    screenShader->build();

    printf("compiling effect shader ...\n");
    effectShader = new RawShader(screenVertexShader, spriteFragmentShader);
    effectShader->build();

    printf("compiling initial lightning shader ...\n");
    initialLightningShader = new RawShader(screenVertexShader, initialLightningFragmentCode);
    initialLightningShader->build();

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

/*
const char *gShaderFragmentCode =
    "#version 400\n"
    "layout (location = 0) out vec3 gPosition;"
    "layout (location = 1) out vec3 gNormal;"
    "layout (location = 2) out vec4 gAlbedoSpec;"
    "uniform sampler2D TextureDefuse;\n"
    "uniform sampler2D TextureSpecular;\n"
    "in vec2 TexCoords;"
    "in vec3 FragPos;"
    "in vec3 Normal;"
    "void main() {\n"
    "   gPosition = FragPos;\n"
    "   gNormal = Normal;\n"
    "   gAlbedoSpec.rgb = texture(TextureDefuse, TexCoords).rgb;\n"
    "   gAlbedoSpec.a = texture(TextureSpecular, TexCoords).r;\n"
    "}\n";
*/

const char *meshLightningFragmentCode =
    "#version 400\n"
    "out vec4 FragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D tPosition;\n"
    "uniform sampler2D tNormal;\n"
    "uniform sampler2D tAlbedoSpec;\n"
    "uniform vec3 ambientColor;\n"
    "void main() {\n"
    "   vec3 FragPos = texture(tPosition, texCoord).rgb;\n"
    "   vec3 Normal = texture(tNormal, texCoord).rgb;\n"
    "   vec3 lightDir = normalize(vec3(-1.0, 1.0, 1.0));"
    "   vec3 Albedo = texture(tAlbedoSpec, texCoord).rgb;\n"
    "   float Specular = texture(tAlbedoSpec, texCoord).a;\n"
    "   vec3 diffuse = Albedo * ambientColor;\n"
    "   vec3 light = max(dot(Normal, lightDir), 0.0) * Albedo;\n"
    "   FragColor = vec4(diffuse + light, 1.0);\n"
    "}\n";

const char *initialLightningFragmentCode =
    "#version 400\n"
    "out vec4 FragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D tAlbedo;\n"
    "uniform sampler2D tNormal;\n"
    "uniform vec3 vf3ambientColor;\n"
    "void main() {\n"
    "   vec3 Albedo = texture(tAlbedo, texCoord).rgb;\n"
    "   vec3 Normal = texture(tNormal, texCoord).rgb;\n"
    "   FragColor = length(Normal) == 0 ? vec4(0, 0, 0, 0) : vec4(Albedo * vf3ambientColor, 1.0);\n"
    "}\n";

const char *sunFragmentCode =
    "#version 400\n"
    "out vec4 FragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D tAlbedoSpec;\n"
    "uniform sampler2D tNormal;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 lightDir;\n"
    "void main() {\n"
    "   vec3 Normal = texture(tNormal, texCoord).rgb;\n"
    "   vec3 Albedo = texture(tAlbedoSpec, texCoord).rgb;\n"
    "   vec3 light = max(dot(Normal, lightDir), 0.0) * Albedo * lightColor;\n"
    "   FragColor = vec4(light, 0.0);\n"
    "}\n";

const char *omniFragmentCode =
    "#version 400\n"
    "out vec4 FragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D tPosition;\n"
    "uniform sampler2D tAlbedoSpec;\n"
    "uniform sampler2D tNormal;\n"
    "uniform vec3 lightColor;\n"
    "uniform float affectDistance;\n"
    "uniform mat4 mTransform;\n"
    "void main() {\n"
    "   vec3 FragPos = texture(tPosition, texCoord).rgb;\n"
    "   vec3 Normal = texture(tNormal, texCoord).rgb;\n"
    "   vec3 Albedo = texture(tAlbedoSpec, texCoord).rgb;\n"
    "   vec3 position = vec3(mTransform * vec4(0.0, 0.0, 0.0, 1.0)) * 0.1;\n"
    "   vec3 dif = position - FragPos;"
    "   vec3 lightDir = normalize(dif);\n"
    "   float distPower = max(1.0 - (length(dif) /  affectDistance), 0.0);\n"
    "   vec3 light = max(dot(Normal, lightDir), 0.0) * Albedo * lightColor * distPower;\n"
    "   FragColor = vec4(light, 0.0);\n"
    "}\n";

