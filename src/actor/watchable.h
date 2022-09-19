#pragma once
#include "common/utils.h"
#include "actor/watcher.h"
#include <list>

template <class T>
class Watchable
{
public:
    ~Watchable()
    {
        for (auto it = watchers.begin(); it != watchers.end(); it++)
            (*it)->noticeRemoved();
    }

    Watcher<T> *watch()
    {
        auto watcher = new Watcher<T>((T *)this);
        watchers.push_back(watcher);
        return watcher;
    }

    void removeWatcher(Watcher<T> *watcher)
    {
        for (auto it = watchers.begin(); it != watchers.end(); it++)
            if (*it == watcher)
            {
                watcher->noticeRemoved();
                watchers.erase(it);
                break;
            }
    }

protected:
    std::list<Watcher<T> *> watchers;
};
