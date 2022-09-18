#pragma once
#include "utils.h"
#include "texture.h"
#include "sound.h"
#include "font.h"
#include <vector>

class ResourceManager
{
public:
    ResourceManager();

    EXPORT Texture *addTexture(std::string path);
    EXPORT Texture *getTextureByPath(std::string path);

    EXPORT Sound *addSound(std::string path);
    EXPORT Sound *getSoundByPath(std::string path);

    EXPORT Font *addFont(std::string path, int size);
    EXPORT Font *getFontByPath(std::string path, int size);

    EXPORT void loadAll();

protected:
    std::vector<Texture *> textures;
    std::vector<Sound *> sounds;
    std::vector<Font *> fonts;
};
