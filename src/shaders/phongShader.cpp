// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "phongShader.h"
#include "opengl/glew.h"
#include "opengl/wglew.h"
#include "math/glm/gtc/type_ptr.hpp"

extern const char *gShaderVertexCode;
extern const char *gShaderFragmentCode;

extern const char *gShaderShadowVertexCode;
extern const char *gShaderShadowFragmentCode;

unsigned int PhongShader::currentProgramm = 0;
unsigned int PhongShader::tBlack = 0;
unsigned int PhongShader::tGrey = 0;
unsigned int PhongShader::tZeroNormal = 0;

bool PhongShader::build()
{
    unsigned int vertexShader = 0, fragmentShader = 0, shadowVertexShader = 0, shadowFragmentShader = 0;
    if (!compile(GL_VERTEX_SHADER, gShaderVertexCode, &vertexShader))
        return false;

    if (!compile(GL_FRAGMENT_SHADER, gShaderFragmentCode, &fragmentShader))
        return false;

    if (!compile(GL_VERTEX_SHADER, gShaderShadowVertexCode, &shadowVertexShader))
        return false;

    if (!compile(GL_FRAGMENT_SHADER, gShaderShadowFragmentCode, &shadowFragmentShader))
        return false;

    // Building usual pass programm
    programm = glCreateProgram();
    if (programm == -1)
        return false;

    glAttachShader(programm, fragmentShader);
    glAttachShader(programm, vertexShader);
    glLinkProgram(programm);
    glUseProgram(programm);

    locMViewProjection = glGetUniformLocation(programm, "mViewProjection");
    locMTransform = glGetUniformLocation(programm, "mTransform");
    locMNormal = glGetUniformLocation(programm, "mNormal");
    locTDefuse = glGetUniformLocation(programm, "TextureDefuse");
    locTSpecular = glGetUniformLocation(programm, "TextureSpecular");
    locTNormal = glGetUniformLocation(programm, "TextureNormal");
    locTEmission = glGetUniformLocation(programm, "TextureEmission");

    // Building shadow pass programm
    shadowProgramm = glCreateProgram();
    if (shadowProgramm == -1)
        return false;

    glAttachShader(shadowProgramm, shadowVertexShader);
    glAttachShader(shadowProgramm, shadowFragmentShader);
    glLinkProgram(shadowProgramm);
    glUseProgram(shadowProgramm);

    locShadowMViewProjection = glGetUniformLocation(shadowProgramm, "mViewProjection");
    locShadowMTransform = glGetUniformLocation(shadowProgramm, "mTransform");

    // Grey and Black textures
    if (tBlack == 0)
    {
        const unsigned char blackData[4] = {0x00, 0x00, 0x00, 0xff};

        glGenTextures(1, &tBlack);
        glBindTexture(GL_TEXTURE_2D, tBlack);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blackData);
    }

    if (tGrey == 0)
    {
        const unsigned char greyData[4] = {0xAA, 0xAA, 0xAA, 0xff};
        glGenTextures(1, &tGrey);
        glBindTexture(GL_TEXTURE_2D, tGrey);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, greyData);
    }

    if (tZeroNormal == 0)
    {
        const unsigned char normalData[4] = {0x80, 0x80, 0xff, 0xff};
        glGenTextures(1, &tZeroNormal);
        glBindTexture(GL_TEXTURE_2D, tZeroNormal);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, normalData);
    }

    bIsReady = true;
    return true;
}

void PhongShader::setTexture(TextureType type, Texture *texture)
{
    if (texture)
        texture->reload();

    switch (type)
    {
    case TextureType::Albedo:
        tAlbedo = texture ? texture->getGLTextureId() : 0;
        break;
    case TextureType::Normal:
        tNormal = texture ? texture->getGLTextureId() : 0;
        break;
    case TextureType::Specular:
        tSpecular = texture ? texture->getGLTextureId() : 0;
        break;
    case TextureType::Emission:
        tEmission = texture ? texture->getGLTextureId() : 0;
        break;
    }
}

bool PhongShader::use(Matrix4 mViewProjection, Matrix4 mModel)
{
    if (!bIsReady)
        build();
    if (!bIsReady)
        return false;

    if (currentShader != this || currentProgramm != programm)
    {
        currentShader = this;
        currentProgramm = programm;
        glUseProgram(programm);
    }

    auto mnMatrix = glm::transpose(glm::inverse(mModel));
    if (locMViewProjection != -1)
        glUniformMatrix4fv(locMViewProjection, 1, GL_FALSE, value_ptr(mViewProjection));
    if (locMTransform != -1)
        glUniformMatrix4fv(locMTransform, 1, GL_FALSE, value_ptr(mModel));
    if (locMNormal != -1)
        glUniformMatrix4fv(locMNormal, 1, GL_FALSE, value_ptr(mnMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tAlbedo ? tAlbedo : tGrey);
    glUniform1i(locTDefuse, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tEmission ? tEmission : tBlack);
    glUniform1i(locTEmission, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tNormal ? tNormal : tZeroNormal);
    glUniform1i(locTNormal, 2);

    return true;
}

bool PhongShader::useShadow(Matrix4 mViewProjection, Matrix4 mModel)
{
    if (!bIsReady)
        build();
    if (!bIsReady)
        return false;

    if (currentShader != this || currentProgramm != shadowProgramm)
    {
        currentShader = this;
        currentProgramm = shadowProgramm;
        glUseProgram(shadowProgramm);
    }

    if (locShadowMViewProjection)
        glUniformMatrix4fv(locShadowMViewProjection, 1, GL_FALSE, value_ptr(mViewProjection));
    if (locShadowMTransform)
        glUniformMatrix4fv(locShadowMTransform, 1, GL_FALSE, value_ptr(mModel));

    return true;
}

// Straight go shader
const char *gShaderVertexCode =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "layout (location = 3) in vec3 aTangent;\n"
    "layout (location = 4) in vec3 aBitangent;\n"
    "uniform mat4 mTransform;\n"
    "uniform mat4 mNormal;\n"
    "uniform mat4 mViewProjection;\n"
    "out vec2 TexCoords;\n"
    "out vec3 FragPos;\n"
    "out mat3 mTBN;\n"
    "void main() {\n"
    "   gl_Position = mViewProjection * mTransform * vec4(aPos, 1.0);\n"
    "   FragPos = (mTransform * vec4(aPos, 1.0)).xyz * 0.1;\n"
    "   TexCoords = aTexCoord;\n"
    "   vec3 T = normalize((mNormal * vec4(aTangent,   0.0)).xyz);\n"
    "   vec3 B = normalize((mNormal * vec4(aBitangent, 0.0)).xyz);\n"
    "   vec3 N = normalize((mNormal * vec4(aNormal,    0.0)).xyz);\n"
    "   mTBN = mat3(T, B, N);\n"
    "}\n";

const char *gShaderFragmentCode =
    "#version 400\n"
    "layout (location = 0) out vec4 gAlbedoSpec;"
    "layout (location = 1) out vec3 gNormal;"
    "layout (location = 2) out vec3 gPosition;"
    "layout (location = 3) out vec3 gEmission;"
    "uniform sampler2D TextureDefuse;\n"
    "uniform sampler2D TextureEmission;\n"
    "uniform sampler2D TextureNormal;\n"
    "uniform sampler2D TextureSpecular;\n"
    "in vec2 TexCoords;\n"
    "in vec3 FragPos;\n"
    "in mat3 mTBN;\n"
    "void main() {\n"
    "   gPosition = FragPos;\n"
    "   gNormal = normalize(mTBN * (texture(TextureNormal, TexCoords).rgb * 2.0 - 1.0));\n"
    "   gAlbedoSpec.rgb = texture(TextureDefuse, TexCoords).rgb;\n"
    "   gAlbedoSpec.a = texture(TextureSpecular, TexCoords).r;\n"
    "   gEmission.rgb = texture(TextureEmission, TexCoords).rgb;\n"
    "}\n";

// Shadow go shader
const char *gShaderShadowVertexCode =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "uniform mat4 mTransform;\n"
    "uniform mat4 mViewProjection;\n"
    "void main() {\n"
    "   gl_Position = mViewProjection * mTransform * vec4(aPos, 1.0);\n"
    "}\n";

const char *gShaderShadowFragmentCode =
    "#version 400\n"
    "layout (location = 0) out vec4 gAlbedoSpec;"
    "uniform sampler2D TextureDefuse;\n"
    "uniform sampler2D TextureSpecular;\n"
    "in vec2 TexCoords;"
    "void main() {\n"
    "   gAlbedoSpec.rgb = texture(TextureDefuse, TexCoords).rgb;\n"
    "   gAlbedoSpec.a = texture(TextureSpecular, TexCoords).r;\n"
    "}\n";