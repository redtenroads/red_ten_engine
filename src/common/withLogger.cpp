// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "withLogger.h"

LogController *WithLogger::logger = nullptr;

void WithLogger::setLogController(LogController *newLogger)
{
    logger = newLogger;
}
