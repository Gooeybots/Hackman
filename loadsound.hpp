#ifndef LOADSOUND_HPP
#define LOADSOUND_HPP

#include <AL/alc.h>
#include <string>

void SoundToVec(const std::string &filename, unsigned int &buffer, ALCcontext * context);
unsigned int LoadSound(const char * filename, ALCcontext * context);

#endif // LOADSOUND_HPP
