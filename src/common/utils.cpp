// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "common/utils.h"
#include <stdlib.h>

float randf()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float randf(float from, float to)
{
    float r3 = from + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (to - from)));
}