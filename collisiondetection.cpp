#include "collisiondetection.hpp"
#include "visibleobject.hpp"

CollisionDetection::CollisionDetection(){}

bool CollisionDetection::DetectCollisions()
{
    bool collided(false);
    for(auto player(mPlayers.begin()); player != mPlayers.end() && !collided; ++player)
    {
        int x((*player)->GetX()), y((*player)->GetY());
        for(auto enemy(mEnemys.begin()); enemy != mEnemys.end(); ++enemy)
        {
            int enemyX((*enemy)->GetX()), enemyY((*enemy)->GetY());
            if(enemyX == x && enemyY == y)
                collided = true;
        }
    }
    return collided;
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
        AddEnemy(*enemy);
    }
}

void CollisionDetection::AddPlayers(std::vector<std::shared_ptr<VisibleObject> > &playersVec)
{
    for(auto player(playersVec.begin()); player != playersVec.end(); ++player)
    {
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
            {
                AddPlayer(*player);
            }
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
