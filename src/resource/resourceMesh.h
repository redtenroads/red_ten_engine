// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/withLogger.h"
#include "common/mesh.h"
#include <string>

class ResourceMesh : public Mesh, public WithLogger
{
public:
    EXPORT ResourceMesh(std::string meshPath);
    EXPORT ~ResourceMesh();

    EXPORT void reload();
    EXPORT bool isLoaded();
    EXPORT bool isPath(std::string meshPath);
    EXPORT Geometry *getGeometry();

    // positions
    // x, y, z - nx, ny, nz - u, v
    EXPORT void setupByArray8f(const float *data, int amount);

    EXPORT void use();

protected:
    bool bLoaded = false;
    Geometry *geometry = nullptr;

    std::string meshPath;
};
