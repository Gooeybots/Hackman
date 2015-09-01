#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <textrenderer.hpp>
#include "setupgame.hpp"
#include "mainmenu.hpp"

unsigned int UpdateList(TextRenderer &textRenderer, const unsigned int menuPos,
                        const std::vector<std::string> &stringVec);

void MainMenu()
{
    for(bool playing(true); playing &&
        !glfwWindowShouldClose(glfwGetCurrentContext());)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        TextRenderer textRenderer;
        std::vector<std::string> textVec;
        textVec.push_back("Start");    textVec.push_back("Options");
        textVec.push_back("Highscores");    textVec.push_back("Quit");
        textRenderer.AddTextVerticalAlign(textVec, TextRenderer::Alignment::Center,
                                          TextRenderer::Alignment::Center, 35.0f,
                                          glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        textRenderer.ChangeTextColour("Start", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        unsigned int menuPos(0);
        bool menuOptionSelected(false);
        bool enterPressed(false);
        while(!glfwWindowShouldClose(glfwGetCurrentContext()) &&
              !menuOptionSelected)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            menuPos = UpdateList(textRenderer, menuPos, textVec);
            textRenderer.DrawAll();

            if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS &&
                    !enterPressed)
                enterPressed = true;
            else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_RELEASE &&
                    enterPressed)
            {
                enterPressed = false;
                menuOptionSelected = true;
            }

            glfwSwapBuffers(glfwGetCurrentContext());
            glfwPollEvents();
        }
        if(menuOptionSelected)
        {
            menuOptionSelected = false;
            if(menuPos == 0)
            {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                playing = SetupGame();
            }
            else if(menuPos == 3)
                playing = false;
            else
                playing = false;
        }
    }
    return;
}

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
        switch(menuPos)
        {
        case 0:
            where = 3;
            break;
        default:
            where -= 1;
            break;
        }
    }
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS &&
            !downPressed)
    {
        downPressed = true;
        changed = true;
        switch(menuPos)
        {
        case 3:
            where = 0;
            break;
        default:
            where += 1;
            break;
        }
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
