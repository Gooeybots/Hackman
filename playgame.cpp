#define GLM_FORCE_RADIANS

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "setupobjects.hpp"
#include "playgame.hpp"
#include "map.hpp"
#include "resourcemanager.hpp"
#include "visibleobject.hpp"
#include "character.hpp"

struct Movement
{
    unsigned int player;
    Direction dir;
};

void GetInput(Movement &movement);

bool PlayGame(Map map, ResourceManager resourceManager)
{
    glm::mat4 view(glm::ortho(0.0f, 40.0f, 0.0f, 30.0f, 1.0f, -1.0f));
    std::vector<std::shared_ptr<VisibleObject>> objectVec;
    Movement movement;
    movement.player = 1;

    bool playing(true);
    while(playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        double currTime(glfwGetTime()), prevTime(currTime);
        GetVecFullOfObjects(objectVec, map, resourceManager);
        for(bool nextMap(false); !nextMap &&
            !glfwWindowShouldClose(glfwGetCurrentContext());)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            GetInput(movement);

            for(auto & obj : objectVec)
            {
                if(movement.player == obj->GetPlayer())
                    if(obj->Move(movement.dir, currTime - prevTime, map))
                        map.SetObject(
                                    glm::ivec2((int)(obj->GetX() + 0.5f),
                                               (int)(obj->GetY() + 0.5f)),
                                    obj->GetPlayer());
                obj->Draw(view);
            }

            glfwSwapBuffers(glfwGetCurrentContext());
            glfwPollEvents();

            prevTime = currTime;
            currTime = glfwGetTime();
        }
    }

    return false;
}

void GetInput(Movement &movement)
{
    if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
        movement.dir = Direction::Left;
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        movement.dir = Direction::Right;
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
        movement.dir = Direction::Up;
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)
        movement.dir = Direction::Down;
}
