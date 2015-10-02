#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include "loadsound.hpp"
#include "oggdecoder.hpp"

void SoundToVec(const std::string &filename, unsigned int &buffer, ALCcontext * context)
{
    buffer = LoadSound(filename.c_str(), context);
}

unsigned int LoadSound(const char * filename, ALCcontext * context)
{
    alcMakeContextCurrent(context);

    OggDecoder decoder(filename);
    decoder.GetInfo();
    decoder.FillBuffer();

    unsigned int buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, decoder.GetChannels() > 1 ?
                     AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
                 decoder.GetData(), decoder.GetBufferSize(), decoder.GetSampleRate());
    decoder.ClearBuffer();

    return buffer;
}
