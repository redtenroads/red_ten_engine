// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "component/component.h"
#include "controller/soundController.h"
#include "resource/sound.h"

class ComponentSoundPlayer : public Component
{
public:
    EXPORT ComponentSoundPlayer();
    EXPORT ~ComponentSoundPlayer();

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
    EXPORT virtual void render(Matrix4 &vpMatrix, Transformation *tf);

    static void setSoundController(SoundController *soundController);

protected:
    Sound *sound = nullptr;
    SoundStream *soundStream = nullptr;
    unsigned int sourceID;
    unsigned int sourceIDSecond;
    int state = 0;
    static SoundController *soundController;
    float maxDistance = 4000.0f;
    float referenceDistance = 2000.0f;
    bool repeatStream = false;
};
