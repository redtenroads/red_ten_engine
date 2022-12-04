// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/childProcess.h"
#include "math/math.h"
#include "controller/soundController.h"
#include "resource/sound.h"

class SoundPlayer : public ChildProcess
{
public:
    EXPORT SoundPlayer();
    EXPORT ~SoundPlayer();

    EXPORT void setPosition(Vector3 position);
    EXPORT void setPosition(float x, float y, float z);

    EXPORT void setSound(Sound *sound);
    EXPORT void playOnce();
    EXPORT void playLoop();
    EXPORT void stop();
    EXPORT bool isPlaying();
    EXPORT bool isLooping();

    EXPORT void setMaxDistance(float maxDistance);
    EXPORT float getMaxDistance();
    EXPORT void setRefereneceDistance(float referenceDistance);
    EXPORT float getReferenceDistance();
    EXPORT void setVolume(float volume);
    EXPORT float getVolume();

    EXPORT void process(float delta);
    static void setSoundController(SoundController *soundController);

protected:
    Sound *sound = nullptr;
    SoundStream *soundStream = nullptr;
    unsigned int sourceID;
    unsigned int sourceIDSecond;

    int state = 0;
    float maxDistance = 4000.0f;
    float referenceDistance = 2000.0f;
    bool repeatStream = false;

    static SoundController *soundController;
};