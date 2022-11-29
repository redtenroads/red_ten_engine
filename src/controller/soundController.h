// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/withLogger.h"
#include <vector>

struct AudioDevice
{
    const char *name;
};

class SoundController : WithLogger
{
public:
    SoundController();

    void setListenerCamera(void *listenerCamera);
    void *getListenerCamera();
protected:
    bool bSoundEnabled = false;
    std::vector<AudioDevice *> devicesList;

    void *listenerCamera = nullptr;
};
