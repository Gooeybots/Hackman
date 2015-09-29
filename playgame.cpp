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
#include "highscoremenu.hpp"

struct Movement
{
    unsigned int player;
    Direction dir;
};

unsigned int UpdateSpecialItem(std::vector<std::shared_ptr<VisibleObject>> &objVec, Map &map,
                               const unsigned int requiredScore, ResourceManager &resMan);
unsigned int UpdateAiSwitchVec(std::vector<float> &timeSwitch, const unsigned int timesToSwitch,
                               const unsigned int level);
void GetPlayerLives(std::vector<std::shared_ptr<VisibleObject>> &objVec,
                    std::vector<std::string> &livesVec);
bool UpdateLives(unsigned int &lives, TextRenderer &textRenderer, std::vector<std::string> &strVec);
void UpdateScore(const unsigned int updated, TextRenderer &textRenderer, std::vector<std::string> &strVec);
bool GetInput(std::vector<Movement> &movement, std::vector<std::shared_ptr<AI>> &aiVec, Map &map);
bool UpdateObject(const glm::ivec2 &where, Map &map, const unsigned int player,
                  std::vector<std::shared_ptr<VisibleObject>> &objVec);
bool UpdateMap(std::shared_ptr<VisibleObject> &player,
               std::vector<std::shared_ptr<VisibleObject>> &objVec,
               Map &map, unsigned int &score);
void UpdatePlayerMovement(std::shared_ptr<VisibleObject> &player, std::vector<Movement> &moveVec,
                                     Map &map, const float deltaTime);
bool AISwitchModes(std::vector<std::shared_ptr<AI>> &aiVec, const float timeSinceStart, const float &timesToChange);
void UpdateEnemysDieStatus(std::vector<std::shared_ptr<VisibleObject>> &objVec,
                           ResourceManager &resMan);

bool PlayGame(Map &map, ResourceManager &resourceManager, const unsigned int livesIn)
{
    bool nextMap(true), menu(true);
    unsigned int lives(livesIn);
    unsigned int specialItemScoreRequired(1000);
    unsigned int score(0), changedScore(score);
    TextRenderer textRender(resourceManager.GetTexture("text.png"));

    std::vector<std::string> stringVec{"Score:", "0"};
    std::vector<std::string> livesVec{"Lives: ", std::to_string(lives)};

    textRender.AddTextVerticalAlign(stringVec, TextRenderer::Alignment::Right, TextRenderer::Alignment::Top, 20.0f);
    textRender.AddTextHorizontalAlign(livesVec, TextRenderer::Alignment::Right, TextRenderer::Alignment::Bottom, 20.0f);

    glm::mat4 view(glm::ortho(0.0f, 40.0f, 0.0f, 30.0f, 1.0f, -1.0f));
    std::vector<std::shared_ptr<VisibleObject>> objectVec;
    std::vector<Movement> movementVec;
    std::vector<std::shared_ptr<AI>> aiVec;
    std::vector<float> aiStateSwitchTimes;
    std::vector<DeadPlayers> deadPlayerVec;

    int level(0);
    int timesToChange(7);

    while(nextMap && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        ++level;
        timesToChange = UpdateAiSwitchVec(aiStateSwitchTimes, timesToChange, level);
        bool playing = true;
        map.LoadNextMap();

        GetVecFullOfObjects(objectVec, map, resourceManager, aiVec);
        textRender.AddTextHorizontalAlign(livesVec, TextRenderer::Alignment::Right, TextRenderer::Alignment::Bottom, 20.0f);

        CollisionDetection collisionDetect;
        collisionDetect.AddPlayersAndEnemys(objectVec);
        map.HideSpecialObj();

        double currTime = glfwGetTime(), prevTime = glfwGetTime(),
                mapStartTime = glfwGetTime(), enemyKillFinish(0.0);

        while(playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
        {

            glClear(GL_COLOR_BUFFER_BIT);

            if(AISwitchModes(aiVec, currTime - mapStartTime, aiStateSwitchTimes.back()))
                aiStateSwitchTimes.erase(aiStateSwitchTimes.end() - 1);
            if(score >= specialItemScoreRequired)
                specialItemScoreRequired = UpdateSpecialItem(objectVec, map, specialItemScoreRequired, resourceManager);
            if(enemyKillFinish != 0.0)
            {
                if(currTime >= enemyKillFinish)
                {
                    UpdateEnemysDieStatus(objectVec, resourceManager);
                    enemyKillFinish = 0.0;
                }
            }
            if(!deadPlayerVec.empty())
            {
                for(auto player(deadPlayerVec.begin()); player != deadPlayerVec.end(); ++player)
                {
                    if((*player).timeEnemyShouldLive <= currTime)
                    {
                        (*player).player->SwitchDeathVao();
                        (*player).player->ResetToOriginalSquare();
                        player = deadPlayerVec.erase(player);
                        --player;
                    }
                }
            }

            if(GetInput(movementVec, aiVec, map))
            {
                if(PauseMenu(resourceManager, menu))
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
                    UpdatePlayerMovement(obj, movementVec, map, currTime - prevTime);
                    if(obj->GetPlayer() == 1)
                    {
                        if(UpdateMap(obj, objectVec, map, changedScore))
                        {
                            enemyKillFinish = currTime;
                            currTime -= 5.0;
                            glfwSetTime(currTime);
                            UpdateEnemysDieStatus(objectVec, resourceManager);
                        }
                        if(score != changedScore)
                        {
                            UpdateScore(changedScore, textRender, stringVec);
                            score = changedScore;

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
            textRender.DrawAll();

            if(collisionDetect.DetectCollisions(lives, changedScore, deadPlayerVec))
            {
                if(UpdateLives(lives, textRender, livesVec))
                {
                    playing = false;
                    nextMap = false;
                    UpdateHighscore(score, resourceManager);
                }
            }

            glfwSwapBuffers(glfwGetCurrentContext());
            glfwPollEvents();

            prevTime = currTime;
            currTime = glfwGetTime();
        }
    }

    return menu;
}

bool UpdateLives(unsigned int &lives, TextRenderer &textRenderer,
                 std::vector<std::string> &strVec)
{
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

bool GetInput(std::vector<Movement> &movementVec, std::vector<std::shared_ptr<AI>> &aiVec, Map &map)
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
        aiMovement.player = which->GetPlayer();
        aiMovement.dir = which->GetMove(map);
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

            map.SetObject(where);
            retValue = true;
            break;
        }
    }
    return retValue;
}

bool UpdateMap(std::shared_ptr<VisibleObject> &player,
               std::vector<std::shared_ptr<VisibleObject> > &objectVec, Map &map,
               unsigned int &score)
{
    bool powerPill(false);
    glm::ivec2 where((int)(player->GetX()),
                     (int)(player->GetY()));
    Object obj(map.GetWhichObject(where));
    if(obj == Object::tree || obj == Object::powerPill || obj == Object::specialObject)
    {
        UpdateObject(where, map, player->GetPlayer(), objectVec);
        switch(obj)
        {
        case Object::tree:
            score += 10;
            break;
        case Object::powerPill:
            score += 10;
            powerPill = true;
            break;
        case Object::specialObject:
            score += 100;
            break;
        default:
            break;
        }
    }
    return powerPill;
}

void UpdatePlayerMovement(std::shared_ptr<VisibleObject> &character,
                          std::vector<Movement> &moveVec,
                          Map &map, const float deltaTime)
{
    for(auto it(moveVec.begin()); it != moveVec.end(); ++it)
    {
        if(it->player == character->GetPlayer())
        {
            character->Move(it->dir, deltaTime, map);
            it = moveVec.erase(it);
            break;
        }
    }
}

bool AISwitchModes(std::vector<std::shared_ptr<AI> > &aiVec, const float timeSinceStart,
                   const float &timeToChange)
{
    bool retValue(false);
    if(timeToChange <= timeSinceStart)
    {
        for(auto & ai : aiVec)
        {
            ai->SwitchStates();
        }
        retValue = true;
    }
    return retValue;
}

unsigned int UpdateAiSwitchVec(std::vector<float> &timeSwitch, const unsigned int timesToSwitch,
                               const unsigned int level)
{
    unsigned int change(timesToSwitch + 2);

    float patrolTime(10.0f / (float)level);
    float chaseTime(20.0f - patrolTime);

    if(change > 27)
        change = 27;

    for(int i(change / 2), j((change / 2) - 1); i > 0;)
    {
        timeSwitch.push_back((patrolTime * i) + (chaseTime * j));
        if(i > j)
            --i;
        else
            --j;
    }
    return change;
}

unsigned int UpdateSpecialItem(std::vector<std::shared_ptr<VisibleObject> > &objVec, Map &map,
                               const unsigned int requiredScore, ResourceManager &resMan)
{
    unsigned int score(requiredScore);
    if(score > 5000)
        score += 5000;
    else
        score *= 1.5;

    std::shared_ptr<unsigned int> texture(resMan.GetTexture("scenery2.png"));
    std::shared_ptr<unsigned int> vao(resMan.GetVao("vao bottom right"));

    for(auto & obj : objVec)
    {
        std::shared_ptr<unsigned int> objVao(obj->GetNextVao());
        if(objVao)
        {
            if(*(obj->GetTexture()) == *texture && *objVao == *vao)
            {
                obj->SwitchVaos();
                map.UpdateSpecialObj(glm::ivec2((int)obj->GetX(), (int)obj->GetY()));
                break;
            }
        }
    }
    return score;
}

void UpdateEnemysDieStatus(std::vector<std::shared_ptr<VisibleObject> > &objVec,
                           ResourceManager &resMan)
{
    std::shared_ptr<unsigned int> wolfTexture(resMan.GetTexture("wolf.png")),
            bearTexture(resMan.GetTexture("bear.png")),
                        snakeTexture(resMan.GetTexture("snake.png"));

    for(auto & enemy : objVec)
    {
        if(*(enemy->GetTexture()) == *bearTexture ||
                *(enemy->GetTexture()) == *wolfTexture ||
                *(enemy->GetTexture()) == *snakeTexture)
        {
            enemy->ChangeCanDie();
        }
    }
}
