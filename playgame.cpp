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
void UpdateObject(const glm::ivec2 &where, Map &map, const unsigned int player,
                  std::vector<std::shared_ptr<VisibleObject>> &objVec);

bool PlayGame(Map map, ResourceManager resourceManager)
{
    glm::mat4 view(glm::ortho(0.0f, 40.0f, 0.0f, 30.0f, 1.0f, -1.0f));
    std::vector<std::shared_ptr<VisibleObject>> objectVec;
    Movement movement;   // Will need to put into a vector when I do AI
    movement.player = 1; // Will have to change this when i incorporate AI

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
                {
                    if(obj->Move(movement.dir, currTime - prevTime, map))
                    {
                        glm::ivec2 where((int)(obj->GetX()),
                                         (int)(obj->GetY()));
                        if(map.GetWhichObject(where) == Object::tree)
                        {
                            UpdateObject(where, map, obj->GetPlayer(), objectVec);
                            if(map.HasFinished())
                            {
                                playing = false;
                                nextMap = true;
                            }
                        }
                    }
                }
                obj->Draw(view);
            }
            if(map.HasFinished())
            {
                nextMap = true;
                playing = false;
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
    else
        movement.dir = Direction::None;
}

void UpdateObject(const glm::ivec2 &where, Map &map, const unsigned int player,
                  std::vector<std::shared_ptr<VisibleObject> > &objVec)
{
    for(auto & obj : objVec)
    {
        if((int)obj->GetX() == where.x &&
                (int)obj->GetY() == where.y && player != obj->GetPlayer())
        {
            obj->SwitchVaos();
            map.SetObject(where, 0);
            break;
        }
    }
}
