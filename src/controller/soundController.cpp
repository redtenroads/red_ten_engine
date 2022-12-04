// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "soundController.h"
#include <al.h>
#include <alc.h>
#include <stdio.h>
#include <string>

SoundController::SoundController()
{
    ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration != AL_FALSE)
    {
        const ALCchar *devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        const ALCchar *device = devices, *next = devices + 1;

        size_t len = 0;

        logger->logf("Audio Devices:");
        while (device && *device != '\0' && next && *next != '\0')
        {
            logger->logf("- %s", device);
            devicesList.push_back(new AudioDevice({_strdup(device)}));

            len = strlen(device);
            device += (len + 1);
            next += (len + 2);
        }
        logger->logff("");
    }

    ALCdevice *device = alcOpenDevice(devicesList.size() > 0 ? devicesList.at(0)->name : nullptr);
    if (!device)
    {
        bSoundEnabled = false;
        return;
    }

    ALCcontext *context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context))
    {
        bSoundEnabled = false;
        return;
    }

    bSoundEnabled = true;
    logger->logff("Sound Enabled using %s", devicesList.at(0)->name);

    ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);
}

void SoundController::process(float delta)
{
    if (soundPlayers.size() > 0)
    {
        for (auto it = soundPlayers.begin(); it != soundPlayers.end(); it++)
            (*it)->process(delta);
    }
}

void SoundController::setListenerCamera(void *listenerCamera)
{
    this->listenerCamera = listenerCamera;
}

void *SoundController::getListenerCamera()
{
    return listenerCamera;
}

void SoundController::subscribeSoundPlayer(ChildProcess *audioSubscribe)
{
    unsubscribeSoundPlayer(audioSubscribe);
    soundPlayers.push_back(audioSubscribe);
}

bool SoundController::unsubscribeSoundPlayer(ChildProcess *audioSubscribe)
{
    auto it = soundPlayers.begin();
    while (it != soundPlayers.end())
        if ((*it) == audioSubscribe)
        {
            soundPlayers.erase(it);
            return true;
        }
    return false;
}
