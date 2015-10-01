#include <iostream>
#include <cstdio>
#include <vorbis/vorbisfile.h>
#include "oggdecoder.hpp"

OggDecoder::OggDecoder(const char * filename, const std::size_t bufferSize)
    : mBufferSize(bufferSize), mBufferAmount(0), mIsVorbis(true), mFilename(filename)
{
    OggVorbis_File oggFile;
    FILE * file = fopen(filename, "rb");
    if(ov_open_callbacks(file, &oggFile, NULL, 0, OV_CALLBACKS_DEFAULT) < 0)
    {
        mIsVorbis = false;
        std::cerr << "NOT A VORBIS FILE\n";
        ov_clear(&oggFile);
    }
    else
        fclose(file);
}

std::size_t OggDecoder::GetSampleRate()
{
    return mSampleRate;
}

std::size_t OggDecoder::GetChannels()
{
    return mChannels;
}

std::size_t OggDecoder::GetBPS()
{
    return mBps;
}

std::size_t OggDecoder::GetAmountBuffers()
{
    return mBuffer.size();
}

char * OggDecoder::GetData(const std::size_t nextBuffer, std::size_t &sizeOfBuffer)
{
    if(mBuffer.size() <= nextBuffer)
    {
        sizeOfBuffer = 0;
        return nullptr;
    }
    else if(mBuffer.size() <= nextBuffer + sizeOfBuffer)
        sizeOfBuffer = mBuffer.size() - nextBuffer;

    return (mBuffer.data() + nextBuffer);
}

void OggDecoder::GetInfo()
{
    OggVorbis_File oggFile;
    if(mIsVorbis)
    {
        FILE * file = fopen(mFilename, "rb");
        ov_open(file, &oggFile, NULL, 0);
        vorbis_info * info;
        info = ov_info(&oggFile, -1);

        mChannels = info->channels;
        mSampleRate = info->rate;
        mBps = info->bitrate_nominal;

        ov_clear(&oggFile); // closes file
    }
}

void OggDecoder::FillBuffer()
{
    if(mIsVorbis)
    {
        FILE * file = fopen(mFilename, "rb");
        int bitStream;
        long bytes;
        OggVorbis_File oggFile;
        ov_open(file, &oggFile, NULL, 0);
        do
        {
            char arrBuffer[mBufferSize];
            bytes = ov_read(&oggFile, arrBuffer, mBufferSize, 0, 2, 1, &bitStream);
            mBuffer.insert(mBuffer.end(), arrBuffer, arrBuffer + bytes);
        }while(bytes > 0);
        ov_clear(&oggFile); // closes file
    }
}
