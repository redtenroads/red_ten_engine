// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"

class ChildProcess
{
public:
    EXPORT virtual void process(float delta);
};
