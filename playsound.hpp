#ifndef PLAYSOUND_HPP
#define PLAYSOUND_HPP

#include <list>
#include <AL/alc.h>
#include <mutex>

class SoundPlayer
{
public:
    SoundPlayer(ALCcontext * contextIn);
    ~SoundPlayer();

    void AddToPlay(const unsigned int &soundBuffer);
    void CleanUpPlayed();
private:

    void RemoveFromList(const unsigned int &source);

    ALCcontext * mContext;
    std::list<unsigned int> mSoundList;
    std::mutex mMutex;
    std::size_t mListCount;
};

#endif // PLAYSOUND_HPP
