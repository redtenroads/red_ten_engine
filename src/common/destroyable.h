// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"

class Destroyable
{
public:
    EXPORT void destroy();
    EXPORT bool isDestroyed();
    EXPORT virtual void onDestroy();

protected:
    bool bMarkedToDestroy = false;
};
