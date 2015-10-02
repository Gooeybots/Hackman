#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include <thread>
#include "playsound.hpp"
#include "playmusic.hpp"

void PlaySound(const unsigned int &source);

SoundPlayer::SoundPlayer(ALCcontext * contextIn):mContext(contextIn){}

SoundPlayer::~SoundPlayer()
{
    if(mListCount > 0)
    {
        for(auto & soundIt : mSoundList)
        {
            if(soundIt > 0)
            {
                alSourceStop(soundIt);
                alDeleteSources(1, &soundIt);
                --mListCount;
            }
        }
        mSoundList.clear();
    }
}

void SoundPlayer::AddToPlay(const unsigned int &soundBuffer)
{
    alcMakeContextCurrent(mContext);
    unsigned int source = SetupSource();
    mMutex.lock();
    mSoundList.push_back(source);
    mMutex.unlock();
    ++mListCount;
    alSourcei(source, AL_BUFFER, soundBuffer);
    alSourcePlay(source);
    std::thread sound(PlaySound, std::ref(source));
    sound.detach();
    CleanUpPlayed();
}

void SoundPlayer::CleanUpPlayed()
{
    mMutex.lock();
    for(auto sourceIt(mSoundList.begin()); sourceIt != mSoundList.end();)
    {
        ALenum sourceState;
        alGetSourcei(*sourceIt, AL_SOURCE_STATE, &sourceState);
        if(sourceState != AL_PLAYING)
        {
            alDeleteSources(1, &(*sourceIt));
            sourceIt = mSoundList.erase(sourceIt);
            --mListCount;
        }
        else
            ++sourceIt;
    }
    mMutex.unlock();
}

void PlaySound(const unsigned int &source)
{
    ALenum sourceState;
    do
    {
        alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
    }while(sourceState == AL_PLAYING && alIsSource(source));
}

void SoundPlayer::RemoveFromList(const unsigned int &source)
{
    if(mListCount > 0)
    {
        mMutex.lock();
        for(auto sourceIt(mSoundList.begin()); sourceIt != mSoundList.end(); ++sourceIt)
        {
            if(*(sourceIt) == source)
            {
                alDeleteSources(1, &source);
                mSoundList.erase(sourceIt);
                --mListCount;
                break;
            }
        }
        mMutex.unlock();
    }
}
