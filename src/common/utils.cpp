// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "common/utils.h"
#include "common/mesh.h"
#include "math/math.h"
#include <stdlib.h>

float randf()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float randf(float from, float to)
{
    return from + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (to - from)));
}

void calcTangets(const float *v1, const float *v2, const float *v3, float *out)
{
    // calculate tangetns and bitangents
    Vector3 edge1(v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]);
    Vector3 edge2(v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2]);
    Vector2 deltaUV1(v2[6] - v1[6], v2[7] - v1[7]);
    Vector2 deltaUV2(v3[6] - v1[6], v3[7] - v1[7]);

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    out[0] = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    out[1] = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    out[2] = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    out[3] = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    out[4] = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    out[5] = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
}