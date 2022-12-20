// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "withLogger.h"

LogController *WithLogger::logController = nullptr;

WithLogger::WithLogger()
{
    this->logger = WithLogger::logController;
}

void WithLogger::setLogController(LogController *logController)
{
    WithLogger::logController = logController;
}
