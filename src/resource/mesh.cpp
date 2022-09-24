// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "resource/mesh.h"
#include "opengl/glew.h"
#include "loaders/fbx_loader.h"
#include <string.h>

Mesh::Mesh()
{
}

Mesh::Mesh(std::string meshPath)
{
    this->meshPath = meshPath;
}

void Mesh::reload()
{
    if (meshPath.length() > 0 && !bLoaded)
    {
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
            printf("Failed loading %s\n", meshPath.c_str());
        }
        delete loader;
    }
}

bool Mesh::isLoaded()
{
    return bLoaded;
}

bool Mesh::isPath(std::string meshPath)
{
    return this->meshPath == meshPath;
}

int Mesh::getFloatsAmount()
{
    return floatsAmount;
}

void Mesh::setupByArray8f(const float *data, int amount)
{
    vbo = makeVBO(data, amount);
    vao = makeVAO(vbo);

    bSettedUp = true;

    bLoaded = true;
    floatsAmount = amount;
}

void Mesh::use()
{
    if (!bLoaded)
        reload();

    if (vao)
        glBindVertexArray(vao);
}

unsigned int Mesh::makeVBO(const float *data, int amount)
{
    unsigned int r = 0;
    glGenBuffers(1, &r);
    glBindBuffer(GL_ARRAY_BUFFER, r);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(float), data, GL_STATIC_DRAW);
    return r;
}

unsigned int Mesh::makeVAO(int vbo)
{
    const int pieceSize = 8;
    unsigned int r = 0;
    glGenVertexArrays(1, &r);
    glBindVertexArray(r);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, pieceSize * sizeof(float), NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, pieceSize * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, pieceSize * sizeof(float), (void *)(6 * sizeof(float)));
    return r;
}
