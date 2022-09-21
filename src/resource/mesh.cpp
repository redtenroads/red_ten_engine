// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "resource/mesh.h"
#include "opengl/glew.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::string meshPath)
{
    this->meshPath = meshPath;
}

void Mesh::reload()
{
}

bool Mesh::isLoaded()
{
    return true;
}

bool Mesh::isPath(std::string meshPath)
{
    return this->meshPath == meshPath;
}

void Mesh::setupByArray5f(const float *data, int amount)
{
    vbo = makeVBO(data, amount);
    vao = makeVAO(vbo, true);

    bHasUV = true;
    entrySize = 5;
    bSettedUp = true;
}

void Mesh::setupByArray3f(const float *data, int amount)
{
    vbo = makeVBO(data, amount);
    vao = makeVAO(vbo, false);

    bHasUV = false;
    entrySize = 3;
    bSettedUp = true;
}

void Mesh::use()
{
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

unsigned int Mesh::makeVAO(int vbo, bool bSetupUV)
{
    unsigned int r = 0;
    int pieceSize = bSetupUV ? 5 : 3;
    glGenVertexArrays(1, &r);
    glBindVertexArray(r);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, pieceSize * sizeof(float), NULL);
    if (bSetupUV)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, pieceSize * sizeof(float), (void *)(3 * sizeof(float)));
    return r;
}
