// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "resource/texture.h"
#include "resource/sound.h"
#include "resource/resourceFont.h"
#include "resource/resourceMesh.h"
#include <vector>

class ResourceController
{
public:
    ResourceController();

    EXPORT Texture *addTexture(std::string path);
    EXPORT Texture *getTextureByPath(std::string path);

    EXPORT Sound *addSound(std::string path);
    EXPORT Sound *getSoundByPath(std::string path);

    EXPORT ResourceFont *addFont(int size);
    EXPORT ResourceFont *addFont(std::string path, int size);
    EXPORT ResourceFont *getFontByPath(std::string path, int size);

    EXPORT ResourceMesh *addMesh(std::string path);
    EXPORT ResourceMesh *getMeshByPath(std::string path);

    EXPORT void loadAll();

protected:
    std::vector<Texture *> textures;
    std::vector<Sound *> sounds;
    std::vector<ResourceFont *> fonts;
    std::vector<ResourceMesh *> meshes;
};
