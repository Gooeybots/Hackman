#include "window.hpp"
#include "setupgame.hpp"

int main()
{
    if(SetupWindow("Hackman", 1024, 768))
    {
        return SetupGame();
    }
    return 0;
}
