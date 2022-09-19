#include "common/destroyable.h"

EXPORT void Destroyable::destroy()
{
    if (!bMarkedToDestroy)
    {
        bMarkedToDestroy = true;
        onDestroy();
    }
}

EXPORT bool Destroyable::isDestroyed()
{
    return bMarkedToDestroy;
}

EXPORT void Destroyable::onDestroy()
{
}