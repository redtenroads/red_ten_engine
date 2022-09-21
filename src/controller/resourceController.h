// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "resource/texture.h"
#include "resource/sound.h"
#include "resource/font.h"
#include "resource/mesh.h"
#include "resource/shader.h"
#include <vector>

class ResourceController
{
public:
    ResourceController();

    EXPORT Texture *addTexture(std::string path);
    EXPORT Texture *getTextureByPath(std::string path);

    EXPORT Sound *addSound(std::string path);
    EXPORT Sound *getSoundByPath(std::string path);

    EXPORT Font *addFont(std::string path, int size);
    EXPORT Font *getFontByPath(std::string path, int size);

    EXPORT Mesh *addMesh();
    EXPORT Mesh *addMesh(std::string path);
    EXPORT Mesh *getMeshByPath(std::string path);

    EXPORT Shader *addShader(std::string vertexPath, std::string fragmentPath);
    EXPORT Shader *addShader(const char *vertex, const char *fragment);
    EXPORT Shader *getShaderByPath(std::string vertexPath, std::string fragmentPath);
    

    EXPORT void loadAll();

protected:
    std::vector<Texture *> textures;
    std::vector<Sound *> sounds;
    std::vector<Font *> fonts;
    std::vector<Mesh *> meshes;
    std::vector<Shader *> shaders;
};
