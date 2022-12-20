// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "controller/debugController.h"

class WithDebug
{
public:
    WithDebug();

    static void setDebugController(DebugController *debugController);

protected:
    DebugController *debug;
    static DebugController *debugController;
};
