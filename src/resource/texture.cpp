// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "resource/texture.h"
#include "opengl/glew.h"
#include "loaders/stb_image.h"

Texture::Texture(std::string path)
{
    this->path = path;
}

void Texture::bind()
{
    if (!bIsLoaded)
        reload();
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::bind(int slot)
{
    if (!bIsLoaded)
        reload();
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::reload()
{
    logger->logf("Add texture %s", path.c_str());
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
    if (data)
    {
        processBytemaps(data, width, height, nrChannels);

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        bIsLoaded = true;
    }
    else
    {
        logger->logff("ERROR: %s couldn't be loaded\n", path.c_str());
    }
}

void Texture::clear()
{
}

bool Texture::isLoaded()
{
    return bIsLoaded;
}

bool Texture::isPath(std::string path)
{
    return this->path == path;
}

unsigned int Texture::getGLTextureId()
{
    if (!bIsLoaded)
        reload();
    return textureID;
}

int Texture::getWidth()
{
    return width;
}

int Texture::getHeight()
{
    return height;
}

void Texture::processBytemaps(const unsigned char *data, int width, int height, int nrChannels)
{
    if (!bMakeBytemapAlpha && !bMakeFullBytemap)
        return;

    if (!data)
        data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);

    if (data && width && height && nrChannels)
    {
        mapWidth = width / byteMapScale;
        mapHeight = height / byteMapScale;

        if (bytemapFullData)
            free(bytemapFullData);

        if (bytemapAlphaData)
            free(bytemapAlphaData);

        bytemapAlphaData = nullptr;
        bytemapFullData = nullptr;

        if (bMakeBytemapAlpha)
            bytemapAlphaData = (unsigned char *)malloc(mapWidth * mapHeight);

        if (bMakeFullBytemap)
            bytemapFullData = (unsigned char *)malloc(mapWidth * mapHeight * 4);

        if (bMakeBytemapAlpha || bMakeFullBytemap)
        {
            int sourceSize = width * height * 4;
            for (int iy = 0; iy < mapHeight; iy++)
            {
                int shiftY = iy * mapWidth;
                int sourceShiftY = iy * width * byteMapScale;
                for (int ix = 0; ix < mapWidth; ix++)
                {
                    int sourceAddr = ((sourceShiftY + (ix * byteMapScale)) << 2) + 3;
                    if (bMakeBytemapAlpha)
                        bytemapAlphaData[shiftY + ix] = sourceAddr < sourceSize ? data[sourceAddr] : 0;
                    if (bMakeFullBytemap)
                        *((int *)(&bytemapFullData[(shiftY + ix) << 2])) = sourceAddr < sourceSize ? *((int *)(&data[sourceAddr])) : 0;
                }
            }
        }
    }
}

void Texture::setMakeBytemapAlpha(bool state)
{
    bMakeBytemapAlpha = state;
}

void Texture::setMakeBytemapFull(bool state)
{
    bMakeFullBytemap = state;
}

void Texture::setBytemapScale(int scale)
{
    byteMapScale = scale;
}

unsigned char Texture::getProbeBytemapAlpha(int x, int y)
{
    x /= byteMapScale;
    y /= byteMapScale;
    y = (mapHeight - y - 1);
    if (bytemapAlphaData && x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
    {
        return bytemapAlphaData[y * mapWidth + x];
    }
    return 0;
}

unsigned int Texture::getProbeFull(int x, int y)
{
    return 0;
}

const unsigned char *Texture::getBytemapAlphaData()
{
    return bytemapAlphaData;
}

const unsigned char *Texture::getBytemapFullData()
{
    return bytemapFullData;
}
