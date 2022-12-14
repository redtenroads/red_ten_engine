// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/withLogger.h"
#include "common/childProcess.h"
#include "common/config.h"
#include <vector>

struct AudioDevice
{
    const char *name;
};

class SoundController : WithLogger
{
public:
    SoundController(Config *config);
    void update();

    void process(float delta);

    void setListenerCamera(void *listenerCamera);
    void *getListenerCamera();

    void subscribeSoundPlayer(ChildProcess *audioSubscribe);
    bool unsubscribeSoundPlayer(ChildProcess *audioSubscribe);

protected:
    Config *config;
    bool bSoundEnabled = false;
    std::vector<AudioDevice *> devicesList;
    std::vector<ChildProcess *> soundPlayers;

    void *listenerCamera = nullptr;
};
