#include <algorithm>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "resourcemanager.hpp"
#include "readtostream.hpp"
#include "textrenderer.hpp"
#include "highscoremenu.hpp"
#include "writedata.hpp"

static std::string LETTER;

static void GetInput(GLFWwindow * window, int key, int scancode, int action, int mods);
std::string Input(const std::string &name);
bool AddAllTextToHighscores(std::vector<std::string> &strVec);
bool GetScores(std::vector<Scores> &scoreVec);
bool SortByScore(Scores &a, Scores &b);
std::string GetName(ResourceManager &resourceManager);

void UpdateHighscore(const unsigned int score, ResourceManager &resourceManager)
{
    std::vector<Scores> scoreVec;
    if(GetScores(scoreVec))
    {
        bool highscore(false);
        auto where(scoreVec.begin());
        for(; where != scoreVec.end() && !highscore; ++where)
        {
            if(score > (*where).score)
            {
                highscore = true;
            }
        }
        if(highscore)
        {
            Scores scores;
            scores.name = GetName(resourceManager);
            scores.score = score;
            scoreVec.push_back(scores);
            std::sort(scoreVec.begin(), scoreVec.end(), SortByScore);
            scoreVec.erase(scoreVec.end() - 1);

            WriteHighscore(scoreVec);
        }
        scoreVec.clear();
    }
}

void DisplayHighscores(ResourceManager &resourceManager)
{
    TextRenderer textRenderer(resourceManager.GetTexture("text.png"));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    textRenderer.AddText("Press Enter To Return To The Main Menu", TextRenderer::Alignment::Center,
                         TextRenderer::Alignment::Bottom, 20, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    std::vector<std::string> strVec{"Highscores:"};

    if(AddAllTextToHighscores(strVec))
    {
        textRenderer.AddTextVerticalAlign(strVec, TextRenderer::Alignment::Center,
                                          TextRenderer::Alignment::Center, 30,
                                          glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
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

bool SortByScore(Scores &a, Scores &b)
{
    return a.score > b.score ? true : false;
}

bool GetScores(std::vector<Scores> &scoreVec)
{
    std::stringstream data;
    if(ReadToStream("highscores.txt", data))
    {
        while(data)
        {
            Scores scores;
            data >> scores.name >> scores.score;
            scoreVec.push_back(scores);
        }
        scoreVec.erase(scoreVec.end() - 1);
        return true;
    }
    return false;
}

bool AddAllTextToHighscores(std::vector<std::string> &strVec)
{
    std::vector<Scores> scoreVec;
    if(GetScores(scoreVec))
    {
        std::size_t smallestSize(20);
        for(auto & scores : scoreVec)
        {
            std::string name(scores.name), score(std::to_string(scores.score));
            do
            {
                name += " ";
            }while(name.size() + score.size() < smallestSize);

            name += score;
            strVec.push_back(name);
        }
        scoreVec.clear();
        return true;
    }
    return false;
}

std::string GetName(ResourceManager &resourceManager)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    std::string name("");
    std::vector<std::string> strVec{"Name: ", name};
    TextRenderer textRenderer(resourceManager.GetTexture("text.png"));

    textRenderer.AddTextHorizontalAlign(strVec, TextRenderer::Alignment::Center,
                                        TextRenderer::Alignment::Center, 30,
                                        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    bool playing(true);
    glfwSetKeyCallback(glfwGetCurrentContext(), GetInput);
    bool enterPressed(false);

    while(playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        name = Input(strVec.back());

        if(name != strVec.back())
        {
            textRenderer.RemoveText(strVec.back());
            strVec.back() = name;

            textRenderer.AddTextHorizontalAlign(strVec, TextRenderer::Alignment::Center,
                                                TextRenderer::Alignment::Center, 30,
                                                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS &&
                !enterPressed)
        {
            enterPressed = true;
        }
        else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_RELEASE &&
                enterPressed)
        {
            playing = false;
            if(strVec.back().empty())
            {
                strVec.back() = "Player";
            }
        }

        textRenderer.DrawAll();

        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }
    return name;
}

std::string Input(const std::string &name)
{
    std::string str(name);
    if(!LETTER.size() == 0)
    {
        if(LETTER == "-" && !str.empty())
        {
            str.erase(str.size() - 1);
        }
        else if(LETTER != "-")
        {
            str += LETTER;
        }
        LETTER.clear();
    }
    return str;
}

static void GetInput(GLFWwindow * window, int key, int scancode, int action, int mods)
{

    bool shiftPressed(false);
    if(glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        shiftPressed = true;

    if(action == GLFW_PRESS)
    {
        switch(key)
        {
        case GLFW_KEY_A:
            LETTER = "a";
            break;
        case GLFW_KEY_B:
            LETTER = "b";
            break;
        case GLFW_KEY_C:
            LETTER = "c";
            break;
        case GLFW_KEY_D:
            LETTER = "d";
            break;
        case GLFW_KEY_E:
            LETTER = "e";
            break;
        case GLFW_KEY_F:
            LETTER = "f";
            break;
        case GLFW_KEY_G:
            LETTER = "g";
            break;
        case GLFW_KEY_H:
            LETTER = "h";
            break;
        case GLFW_KEY_I:
            LETTER = "i";
            break;
        case GLFW_KEY_J:
            LETTER = "j";
            break;
        case GLFW_KEY_K:
            LETTER = "k";
            break;
        case GLFW_KEY_L:
            LETTER = "l";
            break;
        case GLFW_KEY_M:
            LETTER = "m";
            break;
        case GLFW_KEY_N:
            LETTER = "n";
            break;
        case GLFW_KEY_O:
            LETTER = "o";
            break;
        case GLFW_KEY_P:
            LETTER = "p";
            break;
        case GLFW_KEY_Q:
            LETTER = "q";
            break;
        case GLFW_KEY_R:
            LETTER = "r";
            break;
        case GLFW_KEY_S:
            LETTER = "s";
            break;
        case GLFW_KEY_T:
            LETTER = "t";
            break;
        case GLFW_KEY_U:
            LETTER = "u";
            break;
        case GLFW_KEY_V:
            LETTER = "v";
            break;
        case GLFW_KEY_W:
            LETTER = "w";
            break;
        case GLFW_KEY_X:
            LETTER = "x";
            break;
        case GLFW_KEY_Y:
            LETTER = "y";
            break;
        case GLFW_KEY_Z:
            LETTER = "z";
            break;
        case GLFW_KEY_0:
            LETTER = "0";
            break;
        case GLFW_KEY_1:
            LETTER = "1";
            break;
        case GLFW_KEY_2:
            LETTER = "2";
            break;
        case GLFW_KEY_3:
            LETTER = "3";
            break;
        case GLFW_KEY_4:
            LETTER = "4";
            break;
        case GLFW_KEY_5:
            LETTER = "5";
            break;
        case GLFW_KEY_6:
            LETTER = "6";
            break;
        case GLFW_KEY_7:
            LETTER = "7";
            break;
        case GLFW_KEY_8:
            LETTER = "8";
            break;
        case GLFW_KEY_9:
            LETTER = "9";
        case GLFW_KEY_SPACE:
            LETTER = " ";
            break;
        case GLFW_KEY_BACKSPACE:
            LETTER = "-";
            break;
        default:
            LETTER.clear();
        }
    }

    if(!(LETTER.empty() || LETTER == "-" || LETTER == " "))
    {
        if(shiftPressed)
            LETTER.front() = std::toupper(LETTER.front());
    }
}
