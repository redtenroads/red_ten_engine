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

Font *ResourceController::addFont(std::string path, int size)
{
    Font *font = getFontByPath(path, size);
    if (!font)
    {
        font = new Font(path, size);
        fonts.push_back(font);
    }
    return font;
}

Font *ResourceController::getFontByPath(std::string path, int size)
{
    for (auto it = fonts.begin(); it != fonts.end(); ++it)
    {
        if ((*it)->isPath(path) && (*it)->getSize() == size)
            return (*it);
    }
    return nullptr;
}

Mesh *ResourceController::addMesh()
{
    auto mesh = new Mesh();
    meshes.push_back(mesh);
    return mesh;
}

Mesh *ResourceController::addMesh(std::string path)
{
    auto mesh = new Mesh(path);
    meshes.push_back(mesh);
    return mesh;
}

Mesh *ResourceController::getMeshByPath(std::string path)
{
    for (auto it = meshes.begin(); it != meshes.end(); ++it)
    {
        if ((*it)->isPath(path))
            return (*it);
    }
    return nullptr;
}

Shader *ResourceController::addShader(std::string vertexPath, std::string fragmentPath)
{
    auto shader = new Shader(vertexPath, fragmentPath);
    shaders.push_back(shader);
    return shader;
}

Shader *ResourceController::addShader(const char *vertexShaderString, const char *fragmentShaderString)
{
    auto shader = new Shader(vertexShaderString, fragmentShaderString);
    shaders.push_back(shader);
    return shader;
}

Shader *ResourceController::getShaderByPath(std::string vertexPath, std::string fragmentPath)
{
    for (auto it = shaders.begin(); it != shaders.end(); ++it)
    {
        if ((*it)->isPath(vertexPath, fragmentPath))
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
    for (auto it = shaders.begin(); it != shaders.end(); ++it)
    {
        if (!(*it)->isLoaded())
            (*it)->reload();
    }
}