// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "controller/logController.h"

class WithLogger
{
public:
    static void setLogController(LogController *newLogger);

protected:
    static LogController *logger;
};
