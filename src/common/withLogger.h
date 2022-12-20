// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "controller/logController.h"

class WithLogger
{
public:
    WithLogger();
    static void setLogController(LogController *logController);

protected:
    LogController *logger;
    static LogController *logController;
};
