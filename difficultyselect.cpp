#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include "resourcemanager.hpp"
#include "textrenderer.hpp"
#include "difficultyselect.hpp"
#include "setupgame.hpp"
#include "menuupdate.hpp"
#include <highscoremenu.hpp>

bool DifficultyOption(ResourceManager &resMan, unsigned int menuPos);
unsigned int DifficultyMenu(ResourceManager &resMan);

bool DifficultySelect(ResourceManager &resMan)
{
    unsigned int menuPos(0);
    bool playing(true);
    bool retValue(false);
    while(playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        menuPos = DifficultyMenu(resMan);
        if(menuPos > 0)
        {
            menuPos -= 1;
            retValue = DifficultyOption(resMan, menuPos);
            playing = false;
        }
        else
            playing = false;
    }
    return retValue;
}
unsigned int DifficultyMenu(ResourceManager &resMan)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    TextRenderer textRenderer(resMan.GetTexture("text.png"));

    std::vector<std::string> textVec{"Easy", "Medium", "Hard", "Back To Main Menu"};

    textRenderer.AddTextVerticalAlign(textVec, TextRenderer::Alignment::Center,
                                      TextRenderer::Alignment::Center, 35.0f,
                                      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    textRenderer.ChangeTextColour("Easy", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    textRenderer.AddText("Press Enter To Select Menu Item", TextRenderer::Alignment::Center,
                         TextRenderer::Alignment::Bottom, 20, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    unsigned int menuPos = 0;
    bool menuOptionSelected(false), enterPressed(false);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
        return menuPos + 1;
    return 0;
}

bool DifficultyOption(ResourceManager &resMan, unsigned int menuPos)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    unsigned int lives(0);
    bool playing(true);

    if(menuPos == 0)
    {
        lives = 5;
    }
    else if(menuPos == 1)
    {
            lives = 3;
    }
    else if(menuPos == 2)
    {
        lives = 1;
    }

    if(lives > 0)
    {
        playing = SetupGame(resMan, lives);
        if(playing)
            DisplayHighscores(resMan);
    }

    return playing;
}
