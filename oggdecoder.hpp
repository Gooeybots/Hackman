#ifndef OGGDECODER_HPP
#define OGGDECODER_HPP

#include <cstdio>
#include <string>
#include <vector>
#include <list>
#include <vorbis/vorbisfile.h>

class OggDecoder
{
public:
    OggDecoder(const char * filename, std::size_t bufferSize = 4096);

    std::size_t GetSampleRate();
    std::size_t GetChannels();
    std::size_t GetBPS();
    std::size_t GetBufferSize();
    char * GetData(const std::size_t nextBuffer, std::size_t &sizeOfBuffer);
    char * GetData();
    void GetInfo();
    void FillBuffer();
    void ClearBuffer();
private:
    std::vector<char> mBuffer;
    std::size_t mSampleRate;
    std::size_t mChannels;
    std::size_t mBps;
    std::size_t mBufferSize;
    std::size_t mBufferAmount;
    const char * mFilename;
    bool mIsVorbis;
};

#endif // OGGDECODER_HPP
