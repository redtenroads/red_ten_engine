// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include <string>
#include "common/utils.h"
#include "common/withLogger.h"

class Texture : public WithLogger
{
public:
    EXPORT Texture(std::string path);

    EXPORT void reload();
    EXPORT void clear();
    EXPORT bool isLoaded();
    EXPORT bool isPath(std::string path);

    EXPORT unsigned int getGLTextureId();
    EXPORT int getWidth();
    EXPORT int getHeight();

    EXPORT void processBytemaps(const unsigned char *data = nullptr, int width = 0, int height = 0, int nrChannels = 0);
    EXPORT void setMakeBytemapAlpha(bool state);
    EXPORT void setMakeBytemapFull(bool state);
    EXPORT void setBytemapScale(int scale);
    EXPORT unsigned char getProbeBytemapAlpha(int x, int y);
    EXPORT unsigned int getProbeFull(int x, int y);
    EXPORT const unsigned char *getBytemapAlphaData();
    EXPORT const unsigned char *getBytemapFullData();

protected:
    std::string path;

    int width = 0, height = 0, nrChannels = 0;
    unsigned int textureID = 0;
    bool bIsLoaded = false;

    bool bMakeBytemapAlpha = false;
    bool bMakeFullBytemap = false;
    int byteMapScale = 1;
    int mapWidth = 0, mapHeight = 0;
    unsigned char *bytemapFullData = nullptr;
    unsigned char *bytemapAlphaData = nullptr;
};