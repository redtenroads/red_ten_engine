#include "font.h"
#include "SDL.h"
#include "SDL_ttf.h"

Font::Font(std::string path, int size)
{
    this->path = path;
    this->size = size;

    font = (InternalFont *)TTF_OpenFont(path.c_str(), size);
}

int Font::getSize()
{
    return size;
}

InternalFont *Font::getFont()
{
    return font;
}

bool Font::isPath(std::string path)
{
    return this->path == path;
}