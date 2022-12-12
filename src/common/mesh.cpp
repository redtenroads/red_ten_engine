// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "mesh.h"
#include "math/math.h"
#include <string.h>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    clear();
}

int Mesh::getVertexAmount()
{
    return vertexAmount;
}

int Mesh::getFloatsPerVertex()
{
    return floatsPerVertex;
}

int Mesh::getAttributesAmount()
{
    return attributesAmount;
}

Geometry *Mesh::getGeometry()
{
    return geometry;
}

bool Mesh::isReady()
{
    return vao != 0;
}

void Mesh::setupFloatsArray(const float *data, int vertexAmount, int attributesAmount, int *attributeSize)
{
    clear();

    this->vertexAmount = vertexAmount;
    this->attributesAmount = attributesAmount;

    int floatsPerVertex = 0;
    for (int i = 0; i < attributesAmount; i++)
        floatsPerVertex += attributeSize[i];

    vbo = makeVBO(data, vertexAmount * floatsPerVertex);
    vao = makeVAO(attributesAmount, attributeSize);
}

void Mesh::use()
{
    if (vao)
        glBindVertexArray(vao);
}

void Mesh::clear()
{
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (vao)
        glDeleteVertexArrays(1, &vao);
    vbo = 0;
    vao = 0;
}

unsigned int Mesh::makeVBO(const float *data, int amount)
{
    unsigned int r = 0;
    glGenBuffers(1, &r);
    glBindBuffer(GL_ARRAY_BUFFER, r);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(float), data, GL_STATIC_DRAW);
    return r;
}

unsigned int Mesh::makeVAO(int attributesAmount, int *attributeSize)
{
    unsigned int r = 0;
    glGenVertexArrays(1, &r);
    glBindVertexArray(r);

    for (int i = 0; i < attributesAmount; i++)
        glEnableVertexAttribArray(i);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    int shift = 0;
    for (int i = 0; i < attributesAmount; i++)
    {
        glVertexAttribPointer(i, attributeSize[i], GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void *)(shift * sizeof(float)));
        shift += attributeSize[i];
    }

    return r;
}
