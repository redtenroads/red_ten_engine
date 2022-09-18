#include "resourceManager.h"

ResourceManager::ResourceManager()
{
}

Texture *ResourceManager::addTexture(std::string path)
{
    Texture *texture = getTextureByPath(path);
    if (!texture)
    {
        texture = new Texture(path);
        textures.push_back(texture);
    }
    return texture;
}

Texture *ResourceManager::getTextureByPath(std::string path)
{
    for (auto it = textures.begin(); it != textures.end(); ++it)
    {
        if ((*it)->isPath(path))
            return (*it);
    }
    return nullptr;
}

Sound *ResourceManager::addSound(std::string path)
{
    Sound *sound = getSoundByPath(path);
    if (!sound)
    {
        sound = new Sound(path);
        sounds.push_back(sound);
    }
    return sound;
}

Sound *ResourceManager::getSoundByPath(std::string path)
{
    for (auto it = sounds.begin(); it != sounds.end(); ++it)
    {
        if ((*it)->isPath(path))
            return (*it);
    }
    return nullptr;
}

Font *ResourceManager::addFont(std::string path, int size)
{
    Font *font = getFontByPath(path, size);
    if (!font)
    {
        font = new Font(path, size);
        fonts.push_back(font);
    }
    return font;
}

Font *ResourceManager::getFontByPath(std::string path, int size)
{
    for (auto it = fonts.begin(); it != fonts.end(); ++it)
    {
        if ((*it)->isPath(path) && (*it)->getSize() == size)
            return (*it);
    }
    return nullptr;
}

void ResourceManager::loadAll()
{
    for (auto it = textures.begin(); it != textures.end(); ++it)
    {
        if (!(*it)->isLoaded())
            (*it)->reload();
    }
}