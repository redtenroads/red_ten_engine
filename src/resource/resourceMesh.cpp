// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "resource/resourceMesh.h"
#include "opengl/glew.h"
#include "loaders/fbx_loader.h"
#include "math/math.h"
#include <string.h>

// number of floats in one polygon
// 3 - position
// 3 - normal
// 2 - UV
// 3 - tangent
// 3 - biTangent

#define PSIZE 14

ResourceMesh::ResourceMesh(std::string meshPath)
{
    this->meshPath = meshPath;
}

ResourceMesh::~ResourceMesh()
{
    if (geometry)
        delete geometry;
}

void ResourceMesh::reload()
{
    if (meshPath.length() > 0 && !bLoaded)
    {
        logger->logf("Add mesh %s", meshPath.c_str());

        FBXLoader *loader = new FBXLoader();
        bool result = loader->load(meshPath);
        if (result && loader->root)
        {
            int amountOfFloats;
            float *data = loader->root->getAsFloatArray8f(&amountOfFloats);
            if (data)
            {
                setupByArray8f(data, amountOfFloats);
                bLoaded = true;
            }
            free(data);
        }
        else
        {
            logger->logff("Failed loading %s\n", meshPath.c_str());
        }
        delete loader;
    }
}

bool ResourceMesh::isLoaded()
{
    return bLoaded;
}

bool ResourceMesh::isPath(std::string meshPath)
{
    return this->meshPath == meshPath;
}

void ResourceMesh::setupByArray8f(const float *data, int amount)
{
    // calculating tangents and bitangents
    int amountOfVertexes = amount / 8;
    int fullAmountOfFloats = amountOfVertexes * PSIZE;

    float *fullData = (float *)malloc(sizeof(float) * fullAmountOfFloats * 4);
    for (int iVertex = 0; iVertex < amountOfVertexes; iVertex += 3)
    {
        const float *oldVertex1 = &data[iVertex * 8];
        float *newVertex1 = &fullData[iVertex * PSIZE];
        const float *oldVertex2 = &data[(iVertex + 1) * 8];
        float *newVertex2 = &fullData[(iVertex + 1) * PSIZE];
        const float *oldVertex3 = &data[(iVertex + 2) * 8];
        float *newVertex3 = &fullData[(iVertex + 2) * PSIZE];

        // initial position, normal and UV
        memcpy(newVertex1, oldVertex1, sizeof(float) * 8);
        memcpy(newVertex2, oldVertex2, sizeof(float) * 8);
        memcpy(newVertex3, oldVertex3, sizeof(float) * 8);

        // calculate tangetns and bitangents
        Vector3 edge1(oldVertex2[0] - oldVertex1[0], oldVertex2[1] - oldVertex1[1], oldVertex2[2] - oldVertex1[2]);
        Vector3 edge2(oldVertex3[0] - oldVertex1[0], oldVertex3[1] - oldVertex1[1], oldVertex3[2] - oldVertex1[2]);
        Vector2 deltaUV1(oldVertex2[6] - oldVertex1[6], oldVertex2[7] - oldVertex1[7]);
        Vector2 deltaUV2(oldVertex3[6] - oldVertex1[6], oldVertex3[7] - oldVertex1[7]);

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        newVertex1[8] = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        newVertex1[9] = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        newVertex1[10] = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        newVertex1[11] = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        newVertex1[12] = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        newVertex1[13] = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        memcpy(&newVertex2[8], &newVertex1[8], sizeof(float) * 6);
        memcpy(&newVertex3[8], &newVertex1[8], sizeof(float) * 6);
    }

    int attributeSizes[5] = {3, 3, 2, 3, 3};
    setupFloatsArray(fullData, amountOfVertexes, 5, attributeSizes);
    geometry = new Geometry(fullData, amountOfVertexes, PSIZE, 0);
    free(fullData);

    bLoaded = true;
}

void ResourceMesh::use()
{
    if (!bLoaded)
        reload();

    if (vao)
        glBindVertexArray(vao);
}

/*
unsigned int ResourceMesh::makeVBO(const float *data, int amount)
{
    unsigned int r = 0;
    glGenBuffers(1, &r);
    glBindBuffer(GL_ARRAY_BUFFER, r);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(float), data, GL_STATIC_DRAW);
    return r;
}

unsigned int ResourceMesh::makeVAO(int vbo)
{
    unsigned int r = 0;
    glGenVertexArrays(1, &r);
    glBindVertexArray(r);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, PSIZE * sizeof(float), NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, PSIZE * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, PSIZE * sizeof(float), (void *)(6 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, PSIZE * sizeof(float), (void *)(8 * sizeof(float)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, PSIZE * sizeof(float), (void *)(11 * sizeof(float)));
    return r;
}
*/
