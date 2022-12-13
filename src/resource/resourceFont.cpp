// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "resourceFont.h"
#include "SDL.h"
#include "SDL_ttf.h"

ResourceFont::ResourceFont(std::string path, int size)
{
    this->path = path;
    this->size = size;

    if (path == "default")
    {
        SDL_RWops *rwFromMemory = SDL_RWFromMem((void*)DMSansRegularTTF, DMSansRegularTTFSize);
        font = (InternalFont *)TTF_OpenFontRW(rwFromMemory, 1, size);
    }
    else
    {
        font = (InternalFont *)TTF_OpenFont(path.c_str(), size);
    }
}

int ResourceFont::getSize()
{
    return size;
}

InternalFont *ResourceFont::getFont()
{
    return font;
}

bool ResourceFont::isPath(std::string path)
{
    return this->path == path;
}