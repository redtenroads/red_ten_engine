// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include <string>

struct InternalFont
{
};

class Font
{
public:
    Font(std::string path, int size);
    int getSize();
    InternalFont *getFont();
    bool isPath(std::string path);

protected:
    int size;
    std::string path;
    InternalFont *font;
};