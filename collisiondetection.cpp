#include <GLFW/glfw3.h>
#include "collisiondetection.hpp"
#include "visibleobject.hpp"
#include "textrenderer.hpp"

CollisionDetection::CollisionDetection(){}

bool CollisionDetection::DetectCollisions(unsigned int &lives, unsigned int &score,
                                          std::vector<DeadPlayers> &deadPlayVec)
{
    bool collision(false);
    for(auto player(mPlayers.begin()); player != mPlayers.end(); ++player)
    {
        int x((*player)->GetX()), y((*player)->GetY());
        for(auto enemy(mEnemys.begin()); enemy != mEnemys.end(); ++enemy)
        {
            int enemyX((*enemy)->GetX()), enemyY((*enemy)->GetY());
            if(enemyX == x && enemyY == y)
            {
                if((*enemy)->GetCanDie() && (*enemy)->GetActive() &&
                        (*player)->GetActive())
                {
                    (*enemy)->SwitchDeathVao();
                    score += 1000;
                    DeadPlayers deadPlayer;
                    deadPlayer.player = *enemy;
                    deadPlayer.timeEnemyShouldLive = glfwGetTime() + 5.0f;
                    deadPlayVec.push_back(deadPlayer);
                }
                else
                {
                    if((*enemy)->GetActive() && (*player)->GetActive())
                    {
                        (*player)->SwitchDeathVao();
                        DeadPlayers deadPlayer;
                        deadPlayer.player = *player;
                        deadPlayer.timeEnemyShouldLive = glfwGetTime() + 5.0f;
                        deadPlayVec.push_back(deadPlayer);
                        lives -= 1;
                        collision = true;
                    }
                }
            }
        }
    }
    return collision;
}

void CollisionDetection::AddEnemy(std::shared_ptr<VisibleObject> &enemy)
{
    if(ToAdd(enemy, mEnemys))
        mEnemys.push_back(enemy);
}

void CollisionDetection::AddPlayer(std::shared_ptr<VisibleObject> &player)
{
    if(ToAdd(player, mPlayers))
        mPlayers.push_back(player);
}

void CollisionDetection::AddEnemys(std::vector<std::shared_ptr<VisibleObject> > &enemysVec)
{
    for(auto enemy(enemysVec.begin()); enemy != enemysVec.end(); ++enemy)
    {
        if((*enemy)->GetPlayer() > 1)
            AddEnemy(*enemy);
    }
}

void CollisionDetection::AddPlayers(std::vector<std::shared_ptr<VisibleObject> > &playersVec)
{
    for(auto player(playersVec.begin()); player != playersVec.end(); ++player)
    {
        if((*player)->GetPlayer() > 0 && (*player)->GetPlayer() < 2)
            AddPlayer(*player);
    }
}

void CollisionDetection::AddPlayersAndEnemys(std::vector<std::shared_ptr<VisibleObject> > &objectVec)
{
    for(auto player(objectVec.begin()); player != objectVec.end(); ++player)
    {
        if((*player)->GetPlayer() > 0)
        {
            if((*player)->GetPlayer() == 1)
                AddPlayer(*player);
            else
                AddEnemy(*player);
        }
    }
}

bool CollisionDetection::ToAdd(std::shared_ptr<VisibleObject> &who,
                               std::vector<std::shared_ptr<VisibleObject> > &vec)
{
    bool toAdd(true);
    if(!vec.empty())
    {
        for(auto vecEnemy(vec.begin()); vecEnemy != vec.end() && toAdd; ++vecEnemy)
        {
            if((*vecEnemy) == who)
                toAdd = false;
        }
    }
    return toAdd;
}
