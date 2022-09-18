#include "componentSoundPlayer.h"
#include "al.h"
#include "alc.h"
#include "vector4.h"

SoundManager *ComponentSoundPlayer::soundManager;

ComponentSoundPlayer::ComponentSoundPlayer()
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
}

ComponentSoundPlayer::~ComponentSoundPlayer()
{
    if (soundStream)
        sound->closeBuffer(soundStream);
    alDeleteSources(1, &sourceID);
}

void ComponentSoundPlayer::setSound(Sound *sound)
{
    stop();
    this->sound = sound;
    sound->load();
    if (!sound->isStreamable())
        alSourcei(sourceID, AL_BUFFER, sound->getBuffer());
}

void ComponentSoundPlayer::playOnce()
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

void ComponentSoundPlayer::playLoop()
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

void ComponentSoundPlayer::stop()
{
    if (sound)
    {
        alSourceStop(sourceID);
    }
}

bool ComponentSoundPlayer::isPlaying()
{
    if (sound)
    {
        alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }
    return false;
}

bool ComponentSoundPlayer::isLooping()
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

void ComponentSoundPlayer::setMaxDistance(float maxDistance)
{
    this->maxDistance = maxDistance;
    alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
}

float ComponentSoundPlayer::getMaxDistance()
{
    return maxDistance;
}

void ComponentSoundPlayer::setRefereneceDistance(float referenceDistance)
{
    this->referenceDistance = referenceDistance;
    alSourcef(sourceID, AL_REFERENCE_DISTANCE, referenceDistance);
}

float ComponentSoundPlayer::getReferenceDistance()
{
    return referenceDistance;
}
void ComponentSoundPlayer::setVolume(float volume)
{
    alSourcef(sourceID, AL_GAIN, volume);
}

float ComponentSoundPlayer::getVolume()
{
    float volume;
    alGetSourcef(sourceID, AL_GAIN, &volume);
    return volume;
}

void ComponentSoundPlayer::process(float delta)
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

void ComponentSoundPlayer::render(Matrix4 &vpMatrix, Transformation *tf)
{
    Vector4 v(0, 0, 0, 1.0f);
    v *= *tf->getModelMatrix();
    alSource3f(sourceID, AL_POSITION, -v.x, v.y, v.z);
}

void ComponentSoundPlayer::setSoundManager(SoundManager *soundManager)
{
    ComponentSoundPlayer::soundManager = soundManager;
}