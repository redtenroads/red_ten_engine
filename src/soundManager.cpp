#include "soundManager.h"
#include "al.h"
#include "alc.h"
#include <stdio.h>
#include <string>

SoundManager::SoundManager()
{
    ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration != AL_FALSE)
    {
        const ALCchar *devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        const ALCchar *device = devices, *next = devices + 1;

        size_t len = 0;

        printf("Audio Devices:\n");
        while (device && *device != '\0' && next && *next != '\0')
        {
            printf("%s\n", device);
            len = strlen(device);
            device += (len + 1);
            next += (len + 2);
            devicesList.push_back(new AudioDevice({_strdup(device)}));
        }
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
    printf("Sound Enabled\n");

    ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);
}

void SoundManager::setListenerCamera(void *listenerCamera)
{
    this->listenerCamera = listenerCamera;
}

void *SoundManager::getListenerCamera()
{
    return listenerCamera;
}
