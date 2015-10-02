#include <AL/al.h>
#include <AL/alc.h>
#include <thread>
#include <functional>
#include "window.hpp"
#include "mainmenu.hpp"
#include "playmusic.hpp"

void CleanUpAl();

int main()
{
    if(SetupWindow("Hackman", 1024, 768))
    {
        ALCdevice * device(alcOpenDevice(NULL));
        ALCcontext * context(alcCreateContext(device, NULL));
        alcMakeContextCurrent(context);
        bool playing(true); // dont like this but it works
        std::thread music(PlayMusic, std::ref(playing), context);
        MainMenu();
        playing = false;
        music.join();   // wait until music has finished its clear up
        CleanUpAl();
    }
    return 0;
}

void CleanUpAl()
{
    ALCcontext * context(alcGetCurrentContext());
    if(context != NULL)
    {
        ALCdevice * device(alcGetContextsDevice(context));
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
}
