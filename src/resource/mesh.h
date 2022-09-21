// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include <string>

class Mesh
{
public:
    EXPORT Mesh();
    EXPORT Mesh(std::string meshPath);

    EXPORT void reload();
    EXPORT bool isLoaded();
    EXPORT bool isPath(std::string meshPath);

    // positions
    // x, y, z + u, v
    EXPORT void setupByArray5f(const float *data, int amount);

    // x, y, z
    EXPORT void setupByArray3f(const float *data, int amount);

    EXPORT void use();

protected:
    unsigned int makeVBO(const float *data, int amount);
    unsigned int makeVAO(int vbo, bool bSetupUV);

    bool bSettedUp = false;
    bool bHasUV = false;
    int entrySize = 0;

    unsigned int vbo = 0;
    unsigned int vao = 0;
    unsigned int programm = 0;

    std::string meshPath;
};
