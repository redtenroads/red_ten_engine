// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"

class Geometry
{
public:
    EXPORT Geometry(const float *data, int vertexAmount, int floatsPerVertex, int inVertexShiftToPosition = 0);
    EXPORT ~Geometry();

    EXPORT const float *getData();
    EXPORT int getVertexAmount();

protected:
    float *geometry = nullptr;
    int vertexAmount = 0;
};