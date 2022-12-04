// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "soundPlayer.h"
#include "math/math.h"
#include <al.h>
#include <alc.h>

SoundController *SoundPlayer::soundController = nullptr;

SoundPlayer::SoundPlayer()
{
    alGenSources((ALuint)1, &sourceID);
    alSourcef(sourceID, AL_PITCH, 1);
    alSourcef(sourceID, AL_GAIN, 1);
    alSource3f(sourceID, AL_POSITION, 0, 0, 0);
    alSource3f(sourceID, AL_VELOCITY, 0, 0, 0);
    alSourcei(sourceID, AL_LOOPING, AL_FALSE);

    alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_FALSE);
    alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
    alSourcef(sourceID, AL_REFERENCE_DISTANCE, referenceDistance);

    soundController->subscribeSoundPlayer(this);
}

SoundPlayer::~SoundPlayer()
{
    if (soundStream)
        sound->closeBuffer(soundStream);
    alDeleteSources(1, &sourceID);
    soundController->unsubscribeSoundPlayer(this);
}

void SoundPlayer::setPosition(Vector3 position)
{
    alSource3f(sourceID, AL_POSITION, -position.x, position.y, position.z);
}

void SoundPlayer::setPosition(float x, float y, float z)
{
    alSource3f(sourceID, AL_POSITION, -x, y, z);
}

void SoundPlayer::setSound(Sound *sound)
{
    stop();
    this->sound = sound;
    sound->load();
    if (!sound->isStreamable())
        alSourcei(sourceID, AL_BUFFER, sound->getBuffer());
}

void SoundPlayer::playOnce()
{
    if (sound)
    {
        if (sound->getBuffer() && !sound->isStreamable())
        {
            alSourcei(sourceID, AL_LOOPING, AL_FALSE);
            alSourcePlay(sourceID);
            state = AL_PLAYING;
        }
        if (sound->isStreamable())
        {
            if (soundStream)
                sound->closeBuffer(soundStream);

            soundStream = sound->createNewStream();
            if (soundStream)
            {
                sound->processBuffers(soundStream);
                alSourceQueueBuffers(sourceID, SOUND_BUFFERS_AMOUNT, soundStream->buffers);
                alSourcePlay(sourceID);
                state = AL_PLAYING;
                repeatStream = false;
            }
        }
    }
}

void SoundPlayer::playLoop()
{
    if (sound)
    {
        if (sound->getBuffer() && !sound->isStreamable())
        {
            alSourcei(sourceID, AL_LOOPING, AL_TRUE);
            alSourcePlay(sourceID);
        }
        if (sound->isStreamable())
        {
            if (soundStream)
                sound->closeBuffer(soundStream);

            soundStream = sound->createNewStream();
            if (soundStream)
            {
                sound->processBuffers(soundStream);
                alSourceQueueBuffers(sourceID, SOUND_BUFFERS_AMOUNT, soundStream->buffers);
                alSourcePlay(sourceID);
                state = AL_PLAYING;
                repeatStream = true;
            }
        }
    }
}

void SoundPlayer::stop()
{
    if (sound)
    {
        alSourceStop(sourceID);
    }
}

bool SoundPlayer::isPlaying()
{
    if (sound)
    {
        alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }
    return false;
}

bool SoundPlayer::isLooping()
{
    if (sound)
    {
        if (sound->getBuffer() && !sound->isStreamable())
        {
            int isLooping;
            alGetSourcei(sourceID, AL_LOOPING, &isLooping);
            alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
            return (isLooping != AL_FALSE) && (state == AL_PLAYING);
        }
        if (sound->isStreamable() && repeatStream && state == AL_PLAYING)
        {
            return true;
        }
    }
    return false;
}

void SoundPlayer::setMaxDistance(float maxDistance)
{
    this->maxDistance = maxDistance;
    alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
}

float SoundPlayer::getMaxDistance()
{
    return maxDistance;
}

void SoundPlayer::setRefereneceDistance(float referenceDistance)
{
    this->referenceDistance = referenceDistance;
    alSourcef(sourceID, AL_REFERENCE_DISTANCE, referenceDistance);
}

float SoundPlayer::getReferenceDistance()
{
    return referenceDistance;
}
void SoundPlayer::setVolume(float volume)
{
    alSourcef(sourceID, AL_GAIN, volume);
}

float SoundPlayer::getVolume()
{
    float volume;
    alGetSourcef(sourceID, AL_GAIN, &volume);
    return volume;
}

void SoundPlayer::process(float delta)
{
    if (sound && sound->isStreamable() && state == AL_PLAYING)
    {
        alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    }
    if (sound && sound->isStreamable() && state == AL_PLAYING)
    {
        ALint buffersProcessed = 0;
        alGetSourcei(sourceID, AL_BUFFERS_PROCESSED, &buffersProcessed);

        while (buffersProcessed > 0)
        {
            alSourceUnqueueBuffers(sourceID, 1, &soundStream->buffers[soundStream->lastLoaded % SOUND_BUFFERS_AMOUNT]);
            bool res = sound->processBuffers(soundStream);
            if (res)
            {
                alSourceQueueBuffers(sourceID, 1, &soundStream->buffers[(soundStream->lastLoaded - 1) % SOUND_BUFFERS_AMOUNT]);
            }
            buffersProcessed--;
        }
    }
    if (sound && sound->isStreamable() && state == AL_STOPPED && soundStream)
    {
        if (repeatStream)
        {
            sound->closeBuffer(soundStream);
            soundStream = 0;
        }
        else
        {
            sound->restartBuffer(soundStream);
        }
    }
}

void SoundPlayer::setSoundController(SoundController *soundController)
{
    SoundPlayer::soundController = soundController;
}