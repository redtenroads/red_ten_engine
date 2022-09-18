// supports formats:
// usual: .wav,
// streaming: .ogg

#pragma once
#include "utils.h"
#include <string>

#define SOUND_BUFFER_SIZE (1024 * 128)
#define SOUND_BUFFERS_AMOUNT 4

enum Extension
{
    UNKNOWN = 0,
    WAV,
    OGG
};

struct SoundStream
{
    void *fileReader;
    short *loadBuffer;
    unsigned int buffers[SOUND_BUFFERS_AMOUNT];
    int numChannels;
    int sampleRate;
    int format;
    int lastLoaded;
};

class Sound
{
public:
    EXPORT Sound(std::string path);

    EXPORT bool isPath(std::string path);
    EXPORT bool isStreamable();
    EXPORT bool isLoaded();
    EXPORT void setForceMono(bool state);
    EXPORT unsigned int getBuffer();
    EXPORT SoundStream* createNewStream();
    EXPORT bool processBuffers(SoundStream* stream);
    EXPORT void closeBuffer(SoundStream* stream);
    EXPORT void restartBuffer(SoundStream* stream);
    EXPORT void load();



protected:
    bool loadWAV();
    bool loadOGG();

    std::string path;
    bool bIsStreamable;
    bool bIsLoaded = false;
    unsigned int buffer = 0;
    bool bForceMono = false;
    Extension extension = Extension::UNKNOWN;
};
