#include "window.hpp"
#include "mainmenu.hpp"

int main()
{
    if(SetupWindow("Hackman", 1024, 768))
    {
        MainMenu();
    }
    return 0;
}
