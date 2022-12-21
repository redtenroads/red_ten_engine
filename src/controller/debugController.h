// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/color.h"
#include <vector>
#include <string>

struct DebugEntry
{
    int index;
    int msLeft;
    std::string str;
    Color color;
};

class DebugController
{
public:
    EXPORT void print(std::string str);
    EXPORT void print(Color color, std::string str);
    EXPORT void print(int ms, std::string str);
    EXPORT void print(Color color, int ms, std::string str);

    EXPORT void process(float delta);
    EXPORT std::vector<DebugEntry *> *getEntries();
    EXPORT bool hasIndex(int index);

    const int defaultShowTimeMS = 3000;
    const Color defaultColor{0.5f, 0.9f, 0.5f};

    bool isVisible();
    void enableVisibility();
    void disableVisibility();

protected:
    int index = 0;
    bool bVisible = true;
    std::vector<DebugEntry *> entries;
};
