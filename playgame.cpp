#define GLM_FORCE_RADIANS

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "pausemenu.hpp"
#include "directionenum.hpp"
#include "setupobjects.hpp"
#include "playgame.hpp"
#include "map.hpp"
#include "resourcemanager.hpp"
#include "visibleobject.hpp"
#include "character.hpp"
#include "ai.hpp"
#include "collisiondetection.hpp"
#include "textrenderer.hpp"

struct Movement
{
    unsigned int player;
    Direction dir;
};

void GetPlayerLives(std::vector<std::shared_ptr<VisibleObject>> &objVec,
                    std::vector<std::string> &livesVec);
bool UpdateLives(std::vector<std::shared_ptr<VisibleObject>> &objVec,
                 TextRenderer &textRenderer, std::vector<std::string> &strVec);
void UpdateScore(const unsigned int updated, TextRenderer &textRenderer, std::vector<std::string> &strVec);
bool GetInput(std::vector<Movement> &movement, std::vector<AI> &aiVec, Map &map);
bool UpdateObject(const glm::ivec2 &where, Map &map, const unsigned int player,
                  std::vector<std::shared_ptr<VisibleObject>> &objVec);
bool UpdateTreesAndCheckIfWon(std::shared_ptr<VisibleObject> &player,
                              std::vector<std::shared_ptr<VisibleObject>> &objVec, Map &map,
                              unsigned int &score);
bool UpdatePlayerMovementAndCheckWin(std::shared_ptr<VisibleObject> &player, std::vector<Movement> &moveVec,
                                     std::vector<std::shared_ptr<VisibleObject>> &objVec, Map &map,
                                     const float deltaTime, unsigned int &score);

bool PlayGame(Map map, ResourceManager resourceManager)
{
    bool nextMap(false);
    TextRenderer textRender;
    std::vector<std::string> stringVec;
    std::vector<std::string> livesVec;
    unsigned int score(0);
    stringVec.push_back("Score:");
    stringVec.push_back("0");
    textRender.AddTextVerticalAlign(stringVec, TextRenderer::Alignment::Right, TextRenderer::Alignment::Top, 20.0f);
    glm::mat4 view(glm::ortho(0.0f, 40.0f, 0.0f, 30.0f, 1.0f, -1.0f));
    std::vector<std::shared_ptr<VisibleObject>> objectVec;
    std::vector<Movement> movementVec;
    std::vector<AI> aiVec;

    bool playing(true);
    while(playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        double currTime(glfwGetTime()), prevTime(currTime);
        GetVecFullOfObjects(objectVec, map, resourceManager, aiVec);
        GetPlayerLives(objectVec, livesVec);
        textRender.AddTextHorizontalAlign(livesVec, TextRenderer::Alignment::Right, TextRenderer::Alignment::Bottom, 20.0f);

        CollisionDetection collisionDetect;
        collisionDetect.AddPlayersAndEnemys(objectVec);

        while(!nextMap && !glfwWindowShouldClose(glfwGetCurrentContext()))
        {

            glClear(GL_COLOR_BUFFER_BIT);
            if(GetInput(movementVec, aiVec, map))
            {
                if(PauseMenu())
                {
                    playing = false;
                    nextMap = false;
                    break;
                }
                glfwSetTime(currTime);
            }
            for(auto & obj : objectVec)
            {
                if(obj->GetPlayer() > 0)
                {
                    unsigned int changedScore(score);
                    if(UpdatePlayerMovementAndCheckWin(obj, movementVec, objectVec, map, currTime - prevTime, changedScore))
                    {
                        playing = false;
                        nextMap = true;
                    }
                    if(score != changedScore)
                    {
                        UpdateScore(changedScore, textRender, stringVec);
                        score = changedScore;
                    }
                }
                obj->Draw(view);
            }
            textRender.DrawAll();

            if(collisionDetect.DetectCollisions())
            {
                if(UpdateLives(objectVec, textRender, livesVec))
                {
                    playing = false;
                    nextMap = true;
                }
            }

            glfwSwapBuffers(glfwGetCurrentContext());
            glfwPollEvents();

            prevTime = currTime;
            currTime = glfwGetTime();
        }
    }

    return nextMap;
}

void GetPlayerLives(std::vector<std::shared_ptr<VisibleObject> > &objVec,
                    std::vector<std::string> &livesVec)
{
    livesVec.push_back("Lives: ");
    for(auto & obj : objVec)
    {
        if(obj->GetPlayer() == 1)
            livesVec.push_back(std::to_string(obj->GetLives()));
    }
}

bool UpdateLives(std::vector<std::shared_ptr<VisibleObject> > &objVec,
                 TextRenderer &textRenderer, std::vector<std::string> &strVec)
{
    unsigned int lives(0);
    for(auto & player : objVec)
    {
        if(player->GetPlayer() == 1)
        {
            lives = player->GetLives();
            break;
        }
    }
    textRenderer.RemoveText(*(strVec.begin() + 1));
    (*(strVec.begin() + 1)) = std::to_string(lives);
    textRenderer.AddTextHorizontalAlign(strVec, TextRenderer::Alignment::Right,
                                      TextRenderer::Alignment::Bottom, 20);
    if(lives == 0)
        return true;
    return false;
}

void UpdateScore(const unsigned int updated, TextRenderer &textRenderer,
                 std::vector<std::string> &strVec)
{
    textRenderer.RemoveText(*(strVec.begin() + 1));
    (*(strVec.begin() + 1)) = std::to_string(updated);

    textRenderer.AddTextVerticalAlign(strVec, TextRenderer::Alignment::Right,
                                      TextRenderer::Alignment::Top, 20);
}

bool GetInput(std::vector<Movement> &movementVec, std::vector<AI> &aiVec, Map &map)
{
    static bool pPressed(false);
    bool pause(false);
    if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_P) == GLFW_PRESS && !pPressed)
        pPressed = true;
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_P) == GLFW_RELEASE
            && pPressed)
    {
        pPressed = false;
        pause = true;
    }

    Movement playerMovement;
    playerMovement.player = 1;
    if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
        playerMovement.dir = Direction::Left;
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        playerMovement.dir = Direction::Right;
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
        playerMovement.dir = Direction::Up;
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)
        playerMovement.dir = Direction::Down;
    else
        playerMovement.dir = Direction::None;

    movementVec.push_back(playerMovement);

    for(auto & which : aiVec)
    {
        Movement aiMovement;
        aiMovement.player = which.GetPlayer();
        aiMovement.dir = which.GetMove(map);
        movementVec.push_back(aiMovement);
    }

    return pause;
}

bool UpdateObject(const glm::ivec2 &where, Map &map, const unsigned int player,
                  std::vector<std::shared_ptr<VisibleObject> > &objVec)
{
    bool retValue(false);
    for(auto & obj : objVec)
    {
        if((int)obj->GetX() == where.x &&
                (int)obj->GetY() == where.y && player != obj->GetPlayer())
        {
            obj->SwitchVaos();

            map.SetObject(where, 0);
            retValue = true;
            break;
        }
    }
    return retValue;
}

bool UpdateTreesAndCheckIfWon(std::shared_ptr<VisibleObject> &player,
                              std::vector<std::shared_ptr<VisibleObject> > &objectVec, Map &map,
                              unsigned int &score)
{
    bool finished(false);
    glm::ivec2 where((int)(player->GetX()),
                     (int)(player->GetY()));
    Object obj(map.GetWhichObject(where));
    if(obj == Object::tree || obj == Object::powerPill || obj == Object::specialObject)
    {
        switch(obj)
        {
        case Object::tree:
            if(map.HasFinished())
                finished = true;
            score += 10;
            break;
        case Object::powerPill:
            score += 10;
            break;
        case Object::specialObject:
            score += 100;
            break;
        default:
            break;
        }

        UpdateObject(where, map, player->GetPlayer(), objectVec);
    }
    return finished;
}

bool UpdatePlayerMovementAndCheckWin(std::shared_ptr<VisibleObject> &character, std::vector<Movement> &moveVec,
                                     std::vector<std::shared_ptr<VisibleObject> > &objVec, Map &map,
                                     const float deltaTime, unsigned int &score)
{
    bool complete(false);
    for(auto it(moveVec.begin()); it != moveVec.end(); ++it)
    {
        if(it->player == character->GetPlayer())
        {
            character->Move(it->dir, deltaTime, map);
            it = moveVec.erase(it);
            if(character->GetPlayer() == 1)
                complete = UpdateTreesAndCheckIfWon(character, objVec, map, score);
            break;
        }
    }
    return complete;
}
