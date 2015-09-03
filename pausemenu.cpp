#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <textrenderer.hpp>
#include "setupgame.hpp"
#include "pausemenu.hpp"
#include "menuupdate.hpp"

bool PauseMenu()
{
    bool quit(false);
    bool playing(true);
    while( playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        TextRenderer textRenderer;
        std::vector<std::string> textVec;
        textVec.push_back("Continue");    textVec.push_back("Options");
        textVec.push_back("Quit");
        textRenderer.AddTextVerticalAlign(textVec, TextRenderer::Alignment::Center,
                                          TextRenderer::Alignment::Center, 35.0f,
                                          glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        textRenderer.ChangeTextColour("Continue", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        textRenderer.AddText("Press Enter To Select Menu Item", TextRenderer::Alignment::Center,
                             TextRenderer::Alignment::Bottom, 20, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
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
                playing = false;
            else if(menuPos == 2)
            {
                quit = true;
                playing = false;
            }
            else
                playing = false;
        }
        break;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return quit;
}
