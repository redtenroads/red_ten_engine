#pragma once
#include "utils.h"
#include <vector>

struct AudioDevice
{
    const char *name;
};

class SoundManager
{
public:
    SoundManager();

    void setListenerCamera(void *listenerCamera);
    void *getListenerCamera();
protected:
    bool bSoundEnabled = false;
    std::vector<AudioDevice *> devicesList;

    void *listenerCamera = nullptr;
};
