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
    LogController(std::string filePath);
    bool isReady();
    void logf(const char *format, ...);
    void logff(const char *format, ...);
    void logConsole(const char *format, ...);

private:
    bool bReady = false;
    FILE *logFile = nullptr;

    std::string formatString(const char *format, va_list arg);
    std::string convert(unsigned int num, int base);
};
