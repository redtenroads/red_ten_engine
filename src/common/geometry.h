// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once

class Geometry
{
public:
    Geometry(const float *data, int vertexAmount, int floatsPerVertex, int inVertexShiftToPosition = 0);
    ~Geometry();

    const float *getData();
    int getVertexAmount();

protected:
    float *geometry = nullptr;
    int vertexAmount = 0;
};