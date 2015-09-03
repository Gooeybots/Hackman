#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "readtostream.hpp"
#include "textrenderer.hpp"
#include "highscoremenu.hpp"

bool AddAllTextToHighscores(TextRenderer &textRenderer);

void DisplayHighscores()
{
    TextRenderer textRenderer;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    textRenderer.AddText("Press Enter To Return To The Main Menu", TextRenderer::Alignment::Center,
                         TextRenderer::Alignment::Bottom, 20, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    if(AddAllTextToHighscores(textRenderer))
    {
        bool enterPressed(false);
        for(bool playing(true); playing &&
            !glfwWindowShouldClose(glfwGetCurrentContext());)
        {
            while(!glfwWindowShouldClose(glfwGetCurrentContext()) && playing)
            {
                glClear(GL_COLOR_BUFFER_BIT);

                if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS &&
                        !enterPressed)
                    enterPressed = true;
                else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_RELEASE &&
                        enterPressed)
                    playing = false;

                textRenderer.DrawAll();

                glfwSwapBuffers(glfwGetCurrentContext());
                glfwPollEvents();
            }
        }
    }
    return;
}

bool AddAllTextToHighscores(TextRenderer &textRenderer)
{
    std::stringstream data;
    std::vector<std::string> strVec;

    strVec.push_back("Highscores");

    if(ReadToStream("highscores.txt", data))
    {
        while(data)
        {
            std::string name, score;
            data >> name >> score;
            std::size_t smallestSize(20);
            do
            {
                name += " ";
            }while(name.size() + score.size() < smallestSize);

            name += score;
            strVec.push_back(name);
        }
        textRenderer.AddTextVerticalAlign(strVec, TextRenderer::Alignment::Center,
                                          TextRenderer::Alignment::Center, 30,
                                          glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        return true;
    }
    return false;
}
