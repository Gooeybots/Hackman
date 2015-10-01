#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include "playmusic.hpp"
#include "oggdecoder.hpp"

ALuint SetupSource();
void Play(const ALuint source, bool &playing);
void SetupListener();

void PlayMusic(bool &playing)
{
    ALCdevice * device(alcOpenDevice(NULL));
    if(device)
    {

        ALCcontext * context(alcCreateContext(device, NULL));
        if(alcMakeContextCurrent(context))
        {
            SetupListener();
            ALuint source(SetupSource());
            Play(source, playing);

            alcMakeContextCurrent(NULL);
            alcDestroyContext(context);
        }
        else
            std::cerr << "Failed to make the context current\n";
        alcCloseDevice(device);
    }
    else
        std::cerr << "No device could be created for the background music\n";
}

ALuint SetupSource()
{
    ALuint source;
    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    return source;
}

void Play(const ALuint source, bool &playing)
{
    std::size_t bufferSize(4096), currBuffer(0);

    OggDecoder decoder("music.ogg", bufferSize);
    decoder.GetInfo();
    decoder.FillBuffer();
    const std::size_t sampleRate(decoder.GetSampleRate());

    ALenum format(decoder.GetChannels() > 1 ? AL_FORMAT_STEREO16
                                            : AL_FORMAT_MONO16);
    ALenum sourceState(AL_STOPPED);
    ALuint buffers[3];

    alGenBuffers(3, buffers);
    for(int i(0); i < 3; ++i)
    {
        alBufferData(buffers[i], format, decoder.GetData(currBuffer, bufferSize),
                     bufferSize, sampleRate);
        currBuffer += bufferSize;
    }

    alSourceQueueBuffers(source, 3, buffers);
    alSourcePlay(source);
    while(playing)
    {
        ALint processedBuffer;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processedBuffer);
        while(processedBuffer > 0)
        {
            ALuint buffer;
            alSourceUnqueueBuffers(source, 1, &buffer);
            alBufferData(buffer, format, decoder.GetData(currBuffer, bufferSize),
                         bufferSize, sampleRate);
            if(currBuffer >= decoder.GetAmountBuffers())
            {
                bufferSize = 4096;
                currBuffer = 0;
            }
            else
                currBuffer += bufferSize;

            alSourceQueueBuffers(source, 1, &buffer);
            --processedBuffer;
        }
        alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
        if(sourceState != AL_PLAYING)
            alSourcePlay(source);
    }
    alDeleteSources(1, &source);
    alDeleteBuffers(3, buffers);
}

void SetupListener()
{
    ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 1.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, listenerOri);
}
