// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once

#define CONST_PI 3.1415926f
#define CONST_PI2 6.2831853f
#define EXPORT __declspec(dllexport)

EXPORT float randf();
EXPORT float randf(float from, float to);

EXPORT void calcTangets(const float *v1, const float *v2, const float *v3, float *out);