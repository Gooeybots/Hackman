#ifndef PLAYMUSIC_HPP
#define PLAYMUSIC_HPP

#include <AL/alc.h>

unsigned int SetupSource();
void PlayMusic(bool &playing, ALCcontext * context);

#endif // PLAYMUSIC_HPP
