// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/geometry.h"
#include <string>

class Mesh
{
public:
    EXPORT Mesh();
    EXPORT ~Mesh();
    EXPORT int getVertexAmount();
    EXPORT int getFloatsPerVertex();
    EXPORT int getAttributesAmount();
    EXPORT bool isReady();

    EXPORT void setupFloatsArray(const float *data, int vertexAmount, int attributesAmount, int *attributeSize, bool buildTantents = false);

    EXPORT virtual void use();
    EXPORT virtual void clear();

protected:
    void makeVBO(const float *data, int amount);
    void makeVAO(int attributesAmount, int *attributeSize);

    int vertexAmount = 0;
    int floatsPerVertex = 0;
    int attributesAmount = 0;

    unsigned int vbo = 0;
    unsigned int vao = 0;
    unsigned int programm = 0;

    std::string meshPath;
};
