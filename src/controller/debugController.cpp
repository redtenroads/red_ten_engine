// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "debugController.h"

void DebugController::print(std::string str)
{
    print(defaultColor, defaultShowTimeMS, str);
}

void DebugController::print(Color color, std::string str)
{
    print(color, defaultShowTimeMS, str);
}

void DebugController::print(int ms, std::string str)
{
    print(defaultColor, ms, str);
}

void DebugController::print(Color color, int ms, std::string str)
{
    DebugEntry *entry = new DebugEntry();
    entry->index = index;
    entry->msLeft = ms;
    entry->color = color;
    entry->str = str;
    index++;
    entries.push_back(entry);
}

void DebugController::process(float delta)
{
    auto entry = entries.begin();
    while (entry != entries.end())
    {
        (*entry)->msLeft -= (int)(delta * 1000);
        if ((*entry)->msLeft <= 0)
        {
            delete (*entry);
            entry = entries.erase(entry);
        }
        else
            ++entry;
    }
}

std::vector<DebugEntry *> *DebugController::getEntries()
{
    return &entries;
}

bool DebugController::hasIndex(int index)
{
    for (auto it = entries.begin(); it != entries.end(); it++)
    {
        if ((*it)->index == index)
            return true;
    }
    return false;
}

bool DebugController::isVisible()
{
    return bVisible;
}

void DebugController::enableVisibility()
{
    bVisible = true;
}

void DebugController::disableVisibility()
{
    bVisible = false;
}