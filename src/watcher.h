#pragma once
#include "utils.h"

template <class T>
class Watcher
{
public:
    Watcher(T *target)
    {
        this->target = target;
    }
    ~Watcher()
    {
        if (target)
            printf("ERROR: Watcher removed still targeting watchable\nMake sure to first unwatch this watcher through watchable interface\n");
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
