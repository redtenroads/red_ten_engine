// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "soundController.h"
#include <al.h>
#include <alc.h>
#include <stdio.h>
#include <string>

SoundController::SoundController(Config *config)
{
    std::string prefferedDevice = config->getCurrentAudioDevice();
    ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    bool deviceFound = false;
    bSoundEnabled = false;

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
            if (strcmp(prefferedDevice.c_str(), device) == 0)
                deviceFound = true;

            len = strlen(device);
            device += (len + 1);
            next += (len + 2);
        }
        logger->logff("");
    }

    if (devicesList.size() == 0)
    {
        logger->logff("No sound devices found");
        return;
    }

    if (!deviceFound)
    {
        prefferedDevice = devicesList.at(0)->name;
        config->setCurrentAudioDevice(prefferedDevice);
        logger->logff("Auto selected %s for audio", prefferedDevice.c_str());
    }

    ALCdevice *device = alcOpenDevice(prefferedDevice.c_str());
    if (!device)
    {
        logger->logff("Unable to use %s for audio", prefferedDevice.c_str());
        return;
    }

    ALCcontext *context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context))
    {
        logger->logff("Unable to create audio context");
        return;
    }

    bSoundEnabled = true;
    logger->logff("Sound Enabled using %s", prefferedDevice.c_str());

    ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);
}

void SoundController::update()
{
    
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
