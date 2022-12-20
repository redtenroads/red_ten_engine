// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include <string>
#include <stdio.h>
#include <stdarg.h>

class LogController
{
public:
    EXPORT LogController(std::string filePath);
    EXPORT bool isReady();
    EXPORT void logf(const char *format, ...);
    EXPORT void logff(const char *format, ...);
    EXPORT void logConsole(const char *format, ...);

private:
    bool bReady = false;
    FILE *logFile = nullptr;

    std::string formatString(const char *format, va_list arg);
    std::string convert(unsigned int num, int base);
};
