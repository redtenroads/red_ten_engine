// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "phongShader.h"
#include "opengl/glew.h"
#include "math/glm/gtc/type_ptr.hpp"

extern const char *gShaderVertexCode;
extern const char *gShaderFragmentCode;

bool PhongShader::build()
{
    unsigned int vertexShader = 0, fragmentShader = 0;
    if (!compile(GL_VERTEX_SHADER, gShaderVertexCode, &vertexShader))
        return false;

    if (!compile(GL_FRAGMENT_SHADER, gShaderFragmentCode, &fragmentShader))
        return false;

    programm = glCreateProgram();
    if (programm != -1)
    {
        glAttachShader(programm, fragmentShader);
        glAttachShader(programm, vertexShader);
        glLinkProgram(programm);

        locMViewProjection = glGetUniformLocation(programm, "mViewProjection");
        locMTransform = glGetUniformLocation(programm, "mTransform");
        locMNormal = glGetUniformLocation(programm, "mNormal");
        locTDefuse = glGetUniformLocation(programm, "TextureDefuse");
        locTSpecular = glGetUniformLocation(programm, "TextureSpecular");
        locTNormal = glGetUniformLocation(programm, "TextureNormal");

        bIsReady = true;
        return true;
    }
    return false;
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
    }
}

bool PhongShader::use(Matrix4 mViewProjection, Matrix4 mModel)
{
    if (!bIsReady)
        build();
    if (!bIsReady)
        return false;

    if (currentShader != this)
    {
        currentShader = this;
        glUseProgram(programm);
    }

    auto mnMatrix = glm::transpose(glm::inverse(mModel));
    if (locMViewProjection)
        glUniformMatrix4fv(locMViewProjection, 1, GL_FALSE, value_ptr(mViewProjection));
    if (locMTransform)
        glUniformMatrix4fv(locMTransform, 1, GL_FALSE, value_ptr(mModel));
    if (locMNormal)
        glUniformMatrix4fv(locMNormal, 1, GL_FALSE, value_ptr(mnMatrix));

    if (tAlbedo)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tAlbedo);
        glUniform1i(locTDefuse, 0);
    }

    return true;
}

// mat4 normalMatrix = transpose(inverse(modelView));
const char *gShaderVertexCode =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec2 TexCoords;"
    "out vec3 FragPos;"
    "out vec3 Normal;"
    "uniform mat4 mTransform;\n"
    "uniform mat4 mNormal;\n"
    "uniform mat4 mViewProjection;\n"
    "void main() {\n"
    "   gl_Position = mViewProjection * mTransform * vec4(aPos, 1.0);\n"
    "   FragPos = vec3(mTransform * vec4(aPos, 1.0)) * 0.1;\n"
    "   Normal = normalize(vec3(mNormal * vec4(aNormal, 0.0)));\n"
    "   TexCoords = aTexCoord;\n"
    "}\n";

const char *gShaderFragmentCode =
    "#version 400\n"
    "layout (location = 0) out vec4 gAlbedoSpec;"
    "layout (location = 1) out vec3 gNormal;"
    "layout (location = 2) out vec3 gPosition;"
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