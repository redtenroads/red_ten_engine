// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "resourceController.h"

ResourceController::ResourceController()
{
}

Texture *ResourceController::addTexture(std::string path)
{
    Texture *texture = getTextureByPath(path);
    if (!texture)
    {
        texture = new Texture(path);
        textures.push_back(texture);
    }
    return texture;
}

Texture *ResourceController::getTextureByPath(std::string path)
{
    for (auto it = textures.begin(); it != textures.end(); ++it)
    {
        if ((*it)->isPath(path))
            return (*it);
    }
    return nullptr;
}

Sound *ResourceController::addSound(std::string path)
{
    Sound *sound = getSoundByPath(path);
    if (!sound)
    {
        sound = new Sound(path);
        sounds.push_back(sound);
    }
    return sound;
}

Sound *ResourceController::getSoundByPath(std::string path)
{
    for (auto it = sounds.begin(); it != sounds.end(); ++it)
    {
        if ((*it)->isPath(path))
            return (*it);
    }
    return nullptr;
}

ResourceFont *ResourceController::addFont(int size)
{
    ResourceFont *font = getFontByPath(std::string("default"), size);
    if (!font)
    {
        font = new ResourceFont("default", size);
        fonts.push_back(font);
    }
    return font;
}

ResourceFont *ResourceController::addFont(std::string path, int size)
{
    ResourceFont *font = getFontByPath(path, size);
    if (!font)
    {
        font = new ResourceFont(path, size);
        fonts.push_back(font);
    }
    return font;
}

ResourceFont *ResourceController::getFontByPath(std::string path, int size)
{
    for (auto it = fonts.begin(); it != fonts.end(); ++it)
    {
        if ((*it)->isPath(path) && (*it)->getSize() == size)
            return (*it);
    }
    return nullptr;
}

ResourceMesh *ResourceController::addMesh(std::string path)
{
    auto mesh = new ResourceMesh(path);
    meshes.push_back(mesh);
    return mesh;
}

ResourceMesh *ResourceController::getMeshByPath(std::string path)
{
    for (auto it = meshes.begin(); it != meshes.end(); ++it)
    {
        if ((*it)->isPath(path))
            return (*it);
    }
    return nullptr;
}

void ResourceController::loadAll()
{
    for (auto it = textures.begin(); it != textures.end(); ++it)
    {
        if (!(*it)->isLoaded())
            (*it)->reload();
    }
}