// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "resource/sound.h"
#include "loaders/stb_vorbis.h"
#include <stdio.h>
#include <string>
#include <al.h>
#include <alc.h>

struct WavHeader
{
    char mark[4];
    int fileSize;
    char wave[4];
    char fmt[4];
    int formatLength;
    unsigned short int format;
    unsigned short int numOfChannels;
    int sampleRate;
    int byteRate;
    unsigned short int bytesPerSample;
    unsigned short int bitsPerSample;
    char data[4];
    unsigned int dataSize;
};

Sound::Sound(std::string path)
{
    this->path = path;
    int length = path.length();
    if (length > 5)
    {
        if (
            (path[length - 1] == 'v' || path[length - 1] == 'V') &&
            (path[length - 2] == 'a' || path[length - 2] == 'A') &&
            (path[length - 3] == 'w' || path[length - 3] == 'W') &&
            path[length - 4] == '.')
        {
            logger->logf("%s - wav\n", path.c_str());
            extension = Extension::WAV;
            bIsStreamable = false;
        }

        if (
            (path[length - 1] == 'g' || path[length - 1] == 'G') &&
            (path[length - 2] == 'g' || path[length - 2] == 'G') &&
            (path[length - 3] == 'o' || path[length - 3] == 'O') &&
            path[length - 4] == '.')
        {
            logger->logf("%s - ogg\n", path.c_str());
            extension = Extension::OGG;
            bIsStreamable = true;
        }
    }
}

bool Sound::isPath(std::string path)
{
    return this->path == path;
}

bool Sound::isStreamable()
{
    return bIsStreamable;
}

bool Sound::isLoaded()
{
    return bIsLoaded;
}

void Sound::setForceMono(bool state)
{
    bForceMono = state;
}

unsigned int Sound::getBuffer()
{
    return buffer;
}

SoundStream *Sound::createNewStream()
{
    if (!bIsLoaded)
        return nullptr;

    int error = 0;
    stb_vorbis *vorbis = stb_vorbis_open_filename(path.c_str(), &error, nullptr);

    if (error != 0)
        return nullptr;
    stb_vorbis_info info = stb_vorbis_get_info(vorbis);

    SoundStream *stream = new SoundStream();
    stream->fileReader = vorbis;
    stream->loadBuffer = (short *)malloc(sizeof(short) * SOUND_BUFFER_SIZE * info.channels);
    alGenBuffers((ALuint)SOUND_BUFFERS_AMOUNT, stream->buffers);
    stream->numChannels = info.channels;
    stream->sampleRate = info.sample_rate;
    if (info.channels == 1)
        stream->format = AL_FORMAT_MONO16;
    if (info.channels == 2)
        stream->format = AL_FORMAT_STEREO16;
    stream->lastLoaded = 0;

    return stream;
}

bool Sound::processBuffers(SoundStream *stream)
{
    if (!bIsLoaded || !isStreamable())
        return false;

    int amount = stream->lastLoaded == 0 ? SOUND_BUFFERS_AMOUNT : 1;

    for (int i = 0; i < amount; i++)
    {
        int loaded = stb_vorbis_get_samples_short_interleaved((stb_vorbis *)stream->fileReader, stream->numChannels, stream->loadBuffer, SOUND_BUFFER_SIZE);
        if (loaded)
        {
            alBufferData(
                stream->buffers[stream->lastLoaded % SOUND_BUFFERS_AMOUNT],
                stream->format,
                stream->loadBuffer,
                loaded * stream->numChannels * 2,
                stream->sampleRate);
        }
        else
            return false;
        stream->lastLoaded++;
    }

    return true;
}

void Sound::closeBuffer(SoundStream *stream)
{
    if (!stream)
        return;
    stb_vorbis_close((stb_vorbis *)stream->fileReader);
    alDeleteBuffers(SOUND_BUFFERS_AMOUNT, stream->buffers);
    free(stream);
}

void Sound::restartBuffer(SoundStream *stream)
{
    if (!stream || !bIsLoaded)
        return;

    stb_vorbis_seek_start((stb_vorbis *)stream->fileReader);
    stream->lastLoaded = 0;
}

void Sound::load()
{
    if (bIsLoaded)
        return;

    if (extension == Extension::WAV)
        bIsLoaded = loadWAV();

    if (extension == Extension::OGG)
        bIsLoaded = loadOGG();
}

bool Sound::loadWAV()
{
    FILE *file;
    fopen_s(&file, path.c_str(), "rb");

    if (!file)
        return false;

    WavHeader wavHeader;
    fread(&wavHeader, sizeof(WavHeader), 1, file);

    logger->logf("wav loader - %s\nformat %i, num of channels %i, sampleRate %i, byteRate %i, bytesPerSample %i, bitsPerSample %i, data size %i\n",
           path.c_str(), wavHeader.format, wavHeader.numOfChannels, wavHeader.sampleRate, wavHeader.byteRate, wavHeader.bytesPerSample,
           wavHeader.bitsPerSample, wavHeader.dataSize);

    auto dataSize = wavHeader.dataSize;
    unsigned char *data = (unsigned char *)malloc(dataSize);
    if (!data)
    {
        logger->logff("failed to make buffer of %i\n", dataSize);
        fclose(file);
        return false;
    }
    fread(data, dataSize, 1, file);
    fclose(file);

    if (bForceMono && wavHeader.numOfChannels == 2)
    {
        wavHeader.numOfChannels = 1;
        int newDataSize = dataSize / 2;
        if (wavHeader.bitsPerSample == 8)
            for (int i = 0; i < newDataSize; i++)
                data[i] = data[i << 1];
        else
            for (int i = 0; i < newDataSize / 2; i++)
                *(short int *)(&data[i << 1]) = *(short int *)(&data[(i << 2) + 2]);

        dataSize = newDataSize;
    }

    ALenum format = 0;
    if (wavHeader.numOfChannels == 1)
    {
        if (wavHeader.bitsPerSample == 8)
            format = AL_FORMAT_MONO8;
        if (wavHeader.bitsPerSample == 16)
            format = AL_FORMAT_MONO16;
    }
    if (wavHeader.numOfChannels == 2)
    {
        if (wavHeader.bitsPerSample == 8)
            format = AL_FORMAT_STEREO8;
        if (wavHeader.bitsPerSample == 16)
            format = AL_FORMAT_STEREO16;
    }

    if (!format)
    {
        free(data);
        logger->logff("Unknown sound format in %s\n", path.c_str());
        return false;
    }

    alGenBuffers((ALuint)1, &buffer);
    alBufferData(buffer, format, data, dataSize, wavHeader.sampleRate);
    free(data);
    return true;
}

bool Sound::loadOGG()
{
    FILE *file;
    fopen_s(&file, path.c_str(), "rb");
    if (!file)
        return false;
    fclose(file);
    return true;
}