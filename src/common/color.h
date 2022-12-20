// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"

class Color
{
public:
    EXPORT Color();
    EXPORT Color(float r, float g, float b);
    EXPORT Color(float r, float g, float b, float a);

    float r, g, b, a;

    EXPORT Color &operator=(const Color &other);
};
