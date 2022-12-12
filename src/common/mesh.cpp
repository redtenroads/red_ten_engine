// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "mesh.h"
#include "math/math.h"
#include "opengl/glew.h"
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

bool Mesh::isReady()
{
    return vao != 0;
}

void Mesh::setupFloatsArray(const float *data, int vertexAmount, int attributesAmount, int *attributeSize, bool buildTantents)
{
    clear();

    floatsPerVertex = 0;
    for (int i = 0; i < attributesAmount; i++)
        floatsPerVertex += attributeSize[i];

    this->vertexAmount = vertexAmount;

    if (buildTantents)
    {
        int newFloatsPerVertex = floatsPerVertex + 6;
        float *newData = (float *)malloc(newFloatsPerVertex * vertexAmount * sizeof(float));
        memset(newData, 0, newFloatsPerVertex * vertexAmount * sizeof(float));
        int limit = vertexAmount - (vertexAmount % 3);

        for (int i = 0; i < limit; i += 3)
        {
            const float *oldV1Addr = &data[(i + 0) * floatsPerVertex];
            const float *oldV2Addr = &data[(i + 1) * floatsPerVertex];
            const float *oldV3Addr = &data[(i + 2) * floatsPerVertex];

            float *newV1Addr = &newData[(i + 0) * newFloatsPerVertex];
            float *newV2Addr = &newData[(i + 1) * newFloatsPerVertex];
            float *newV3Addr = &newData[(i + 2) * newFloatsPerVertex];

            memcpy(newV1Addr, oldV1Addr, floatsPerVertex * sizeof(float));
            memcpy(newV2Addr, oldV2Addr, floatsPerVertex * sizeof(float));
            memcpy(newV3Addr, oldV3Addr, floatsPerVertex * sizeof(float));

            float tg[6];
            calcTangets(newV1Addr, newV2Addr, newV3Addr, tg);

            memcpy(&newV1Addr[floatsPerVertex], tg, 6 * sizeof(float));
            memcpy(&newV2Addr[floatsPerVertex], tg, 6 * sizeof(float));
            memcpy(&newV3Addr[floatsPerVertex], tg, 6 * sizeof(float));
        }

        int newAttributeAmount = attributesAmount + 2;
        int *newAttributeSize = (int *)malloc(newAttributeAmount);
        for (int i = 0; i < attributesAmount; i++)
        {
            newAttributeSize[i] = attributeSize[i];
        }
        newAttributeSize[attributesAmount] = 3;
        newAttributeSize[attributesAmount + 1] = 3;

        floatsPerVertex = newFloatsPerVertex;
        makeVBO(newData, vertexAmount * newFloatsPerVertex);
        makeVAO(newAttributeAmount, newAttributeSize);

        this->attributesAmount = newAttributeAmount;
        free(newData);
        free(newAttributeSize);
    }
    else
    {
        this->attributesAmount = attributesAmount;

        makeVBO(data, vertexAmount * floatsPerVertex);
        makeVAO(attributesAmount, attributeSize);
    }
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

void Mesh::makeVBO(const float *data, int amount)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(float), data, GL_STATIC_DRAW);
}

void Mesh::makeVAO(int attributesAmount, int *attributeSize)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    for (int i = 0; i < attributesAmount; i++)
        glEnableVertexAttribArray(i);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    int shift = 0;
    for (int i = 0; i < attributesAmount; i++)
    {
        glVertexAttribPointer(i, attributeSize[i], GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void *)(shift * sizeof(float)));
        shift += attributeSize[i];
    }
}
