// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/withLogger.h"

template <class T>
class Watcher : public WithLogger
{
public:
    Watcher(T *target)
    {
        this->target = target;
    }
    ~Watcher()
    {
        if (target)
            logger->logff("ERROR: Watcher removed still targeting watchable\nMake sure to first unwatch this watcher through watchable interface\n");
    }

    void noticeRemoved()
    {
        target = nullptr;
        if (killed)
            delete this;
    }

    bool hasTarget()
    {
        return target != nullptr;
    }

    T *getTarget()
    {
        return target;
    }

    void kill()
    {
        if (!target)
            delete this;
        else
            killed = true;
    }

protected:
    T *target;
    bool killed = false;
};
