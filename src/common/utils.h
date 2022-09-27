// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once

#define CONST_PI 3.1415926f
#define EXPORT __declspec(dllexport)

EXPORT float randf();
EXPORT float randf(float from, float to);