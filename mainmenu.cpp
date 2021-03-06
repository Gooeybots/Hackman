#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "resourcemanager.hpp"
#include "textrenderer.hpp"
#include "mainmenu.hpp"
#include "menuupdate.hpp"
#include "mapeditor.hpp"
#include "highscoremenu.hpp"
#include "difficultyselect.hpp"

bool MenuOption(ResourceManager &resMan, unsigned int menuPos);
unsigned int DisplayMenu(ResourceManager &resMan);

void MainMenu()
{
    ResourceManager resMan;
    resMan.CreateTexture("text.png");

    unsigned int menuPos(0);
    for(bool playing(true); playing &&
        !glfwWindowShouldClose(glfwGetCurrentContext());)
    {
        menuPos = DisplayMenu(resMan);
        if(menuPos > 0)
        {
            menuPos -= 1;
            playing = MenuOption(resMan, menuPos);
        }
        else
            playing = false;
    }
}

unsigned int DisplayMenu(ResourceManager &resMan)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    TextRenderer textRenderer(resMan.GetTexture("text.png"));

    std::vector<std::string> textVec{"Start", "Options", "Map Editor", "Highscores", "Quit"};

    textRenderer.AddTextVerticalAlign(textVec, TextRenderer::Alignment::Center,
                                      TextRenderer::Alignment::Center, 35.0f,
                                      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    textRenderer.ChangeTextColour("Start", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
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

bool MenuOption(ResourceManager &resMan, unsigned int menuPos)
{
    bool playing(false);
    if(menuPos == 0)
    {
        playing = DifficultySelect(resMan);
    }
    else if(menuPos == 2)
    {
            playing = true;
            MakeMap(resMan);
    }
    else if(menuPos == 3)
    {
        playing = true;
        DisplayHighscores(resMan);
    }
    return playing;
}
