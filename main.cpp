#include <thread>
#include <functional>
#include "window.hpp"
#include "mainmenu.hpp"
#include "playmusic.hpp"

int main()
{
    bool playing(true); // dont like this but it works
    std::thread music(PlayMusic, std::ref(playing));
    if(SetupWindow("Hackman", 1024, 768))
    {
        MainMenu();
        playing = false;
        music.join();   // wait until music has finished its clearup
    }
    return 0;
}
