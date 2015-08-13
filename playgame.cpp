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

bool PlayGame(Map map, ResourceManager resourceManager)
{
    glm::mat4 view(glm::ortho(0.0f, 40.0f, 0.0f, 30.0f, 1.0f, -1.0f));
    std::vector<std::shared_ptr<VisibleObject>> objectVec;

    bool playing(true);
    while(playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        GetVecFullOfObjects(objectVec, map, resourceManager);
        for(bool nextMap(false); !nextMap &&
            !glfwWindowShouldClose(glfwGetCurrentContext());)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            for(auto & obj : objectVec)
                obj->Draw(view);
            glfwSwapBuffers(glfwGetCurrentContext());
            glfwPollEvents();
        }
    }

    return false;
}
