// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "meshDescriptor.h"

MeshDescriptor::MeshDescriptor(MeshDescriptor *parent)
{
    this->parent = parent;
}

void MeshDescriptor::provideVertex(double *list, int countOfDoubles)
{
    vertexAmount = countOfDoubles / 3;
    vertexes = (Vertex *)malloc(sizeof(Vertex) * vertexAmount);

    for (int i = 0; i < vertexAmount; i++)
    {
        int s = i * 3;
        vertexes[i].p[0] = list[s];
        vertexes[i].p[2] = list[s + 1];
        vertexes[i].p[1] = list[s + 2];
    }
}

void MeshDescriptor::providePolygonIndexes(int *list, int countOfInts)
{
    polygonIndexes = (int *)malloc(sizeof(int) * countOfInts);
    memcpy(polygonIndexes, list, sizeof(int) * countOfInts);
    polygonIndexesAmount = countOfInts;
}

void MeshDescriptor::provideUVData(double *list, int countOfDoubles)
{
    uvsAmount = countOfDoubles / 2;
    uvs = (UV *)malloc(sizeof(UV) * uvsAmount);
    for (int i = 0; i < uvsAmount; i++)
    {
        int shift = i * 2;
        uvs[i].y = list[shift];
        uvs[i].x = 1.0f - list[shift + 1];
    }
}

void MeshDescriptor::provideUVIndexes(int *list, int countOfInts)
{
    uvIndexes = (int *)malloc(sizeof(int) * countOfInts);
    memcpy(uvIndexes, list, sizeof(int) * countOfInts);
    uvIndexesAmount = countOfInts;
}

void MeshDescriptor::provideNormals(double *list, int countOfDoubles)
{
    int polyCount = countOfDoubles / 3;
    normals = (Normal *)malloc(polyCount * sizeof(Normal));
    for (int i = 0; i < polyCount; i++)
    {
        int s = i * 3;
        normals[i].n[0] = (float)list[s];
        normals[i].n[2] = (float)list[s + 1];
        normals[i].n[1] = (float)list[s + 2];
    }
}

float *MeshDescriptor::getAsFloatArray8f(int *amountOfFloats)
{
    if (!vertexes || !uvs || !polygonIndexes || !uvIndexes)
        return nullptr;

    std::vector<Polygon> polygons;
    Polygon p;
    int filled = 0;
    for (int i = 0; i < polygonIndexesAmount; i++)
    {
        int index = polygonIndexes[i];
        int uvIndex = uvIndexes[i];
        bool reset = false;
        if (index < 0)
        {
            index = ~index;
            reset = true;
        }

        if (filled == 0)
        {
            p.p1 = vertexes[index];
            p.n1 = normals[i];
            p.uv1 = uvs[uvIndex];
            filled++;
        }
        else if (filled == 1)
        {
            p.p2 = vertexes[index];
            p.n2 = normals[i];
            p.uv2 = uvs[uvIndex];
            filled++;
        }
        else if (filled == 2)
        {
            p.p3 = vertexes[index];
            p.n3 = normals[i];
            p.uv3 = uvs[uvIndex];
            filled++;
            polygons.push_back(p);
        }
        else if (filled == 3)
        {

            p.p2 = p.p1;
            p.n2 = p.n1;
            p.uv2 = p.uv1;
            p.p1 = p.p3;
            p.n1 = p.n3;
            p.uv1 = p.uv3;
            p.p3 = vertexes[index];
            p.n3 = normals[i];
            p.uv3 = uvs[uvIndex];

            Vertex r = p.p1;
            Normal n = p.n1;
            UV uv = p.uv1;

            p.p1 = p.p2;
            p.n1 = p.n2;
            p.uv1 = p.uv2;
            p.p2 = r;
            p.n2 = n;
            p.uv2 = uv;

            polygons.push_back(p);
        }

        if (reset)
            filled = 0;
    }

    const int floatsInPoly = 24;
    const int polyAmount = polygons.size();
    *amountOfFloats = polyAmount * floatsInPoly;
    float *data = (float *)malloc(sizeof(float) * *amountOfFloats);

    int shift = 0;
    for (auto it = polygons.begin(); it != polygons.end(); it++)
    {
        data[shift + 8] = (*it).p1.p[0];
        data[shift + 9] = (*it).p1.p[1];
        data[shift + 10] = (*it).p1.p[2];
        data[shift + 11] = (*it).n1.n[0];
        data[shift + 12] = (*it).n1.n[1];
        data[shift + 13] = (*it).n1.n[2];
        data[shift + 14] = (*it).uv1.y;
        data[shift + 15] = (*it).uv1.x;
        data[shift + 0] = (*it).p2.p[0];
        data[shift + 1] = (*it).p2.p[1];
        data[shift + 2] = (*it).p2.p[2];
        data[shift + 3] = (*it).n2.n[0];
        data[shift + 4] = (*it).n2.n[1];
        data[shift + 5] = (*it).n2.n[2];
        data[shift + 6] = (*it).uv2.y;
        data[shift + 7] = (*it).uv2.x;
        data[shift + 16] = (*it).p3.p[0];
        data[shift + 17] = (*it).p3.p[1];
        data[shift + 18] = (*it).p3.p[2];
        data[shift + 19] = (*it).n3.n[0];
        data[shift + 20] = (*it).n3.n[1];
        data[shift + 21] = (*it).n3.n[2];
        data[shift + 22] = (*it).uv3.y;
        data[shift + 23] = (*it).uv3.x;
        shift += floatsInPoly;
    }

    return data;
}
