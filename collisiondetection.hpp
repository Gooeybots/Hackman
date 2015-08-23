#ifndef COLLISIONDETECTION_HPP
#define COLLISIONDETECTION_HPP

#include <vector>
#include <memory>

class VisibleObject;

class CollisionDetection
{
public:
    CollisionDetection();
// TODO: Take life off the player that has been hit
/* will return true if there is a collision between an enemy and a player */
    bool DetectCollisions();

/* These will add to the correct vector if they are not already present */
    void AddEnemy(std::shared_ptr<VisibleObject> &enemy);
    void AddPlayer(std::shared_ptr<VisibleObject> &player);
    void AddPlayers(std::vector<std::shared_ptr<VisibleObject>> &playersVec);
    void AddEnemys(std::vector<std::shared_ptr<VisibleObject>> &enemysVec);
    void AddPlayersAndEnemys(std::vector<std::shared_ptr<VisibleObject>> &objectVec);

private:
    bool ToAdd(std::shared_ptr<VisibleObject> &who,
               std::vector<std::shared_ptr<VisibleObject>> &vec);

    std::vector<std::shared_ptr<VisibleObject>> mEnemys;
    std::vector<std::shared_ptr<VisibleObject>> mPlayers;
};

#endif // COLLISIONDETECTION_HPP