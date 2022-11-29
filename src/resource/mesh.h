// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/withLogger.h"
#include <string>

class Mesh : public WithLogger
{
public:
    EXPORT Mesh();
    EXPORT Mesh(std::string meshPath);

    EXPORT void reload();
    EXPORT bool isLoaded();
    EXPORT bool isPath(std::string meshPath);

    EXPORT int getFloatsAmount();

    // positions
    // x, y, z - nx, ny, nz - u, v
    EXPORT void setupByArray8f(const float *data, int amount);


    EXPORT void use();

protected:
    unsigned int makeVBO(const float *data, int amount);
    unsigned int makeVAO(int vbo);

    bool bSettedUp = false;
    bool bLoaded = false;

    int floatsAmount = 0;

    unsigned int vbo = 0;
    unsigned int vao = 0;
    unsigned int programm = 0;

    std::string meshPath;
};
