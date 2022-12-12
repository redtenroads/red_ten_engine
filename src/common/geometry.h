// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once

class Geometry
{
public:
    Geometry(float *data, int amountOfVertexes, int floatsPerVertex, int inVertexShiftToPosition = 0);
    ~Geometry();

    float *getData();
    int getVertexAmount();

protected:
    float *geometry = nullptr;
    int amountOfVertexes = 0;
};