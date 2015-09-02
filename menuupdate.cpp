#include <GLFW/glfw3.h>
#include <textrenderer.hpp>
#include "menuupdate.hpp"

unsigned int UpdateList(TextRenderer &textRenderer, const unsigned int menuPos,
                        const std::vector<std::string> &textVec)
{
    static bool upPressed(false), downPressed(false);
    bool changed(false);
    unsigned int where(menuPos);

    if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS &&
            !upPressed)
    {
        upPressed = true;
        changed = true;
        if(menuPos == 0)
            where = textVec.size() - 1;
        else
            where -= 1;
    }
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS &&
            !downPressed)
    {
        downPressed = true;
        changed = true;

        if(menuPos == textVec.size() - 1)
            where = 0;
        else
            where += 1;
    }

    if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_RELEASE &&
            upPressed)
        upPressed = false;
    if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_RELEASE &&
            downPressed)
        downPressed = false;

    if(changed)
    {
        glm::vec4 selected(1.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 unSelected(0.0f, 0.0f, 1.0f, 1.0f);

        auto textIt(textVec.begin());
        textRenderer.ChangeTextColour(*(textIt + where), selected);
        textRenderer.ChangeTextColour(*(textIt + menuPos), unSelected);
    }
    return where;
}
