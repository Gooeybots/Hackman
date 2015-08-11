#include <GLFW/glfw3.h>
#include "setupgame.hpp"

int SetupGame()
{
    for(bool playing(true); playing &&
        !glfwWindowShouldClose(glfwGetCurrentContext());)
    {
        glfwPollEvents();
    }
    return 0;
}
