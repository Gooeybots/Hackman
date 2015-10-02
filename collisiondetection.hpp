#ifndef COLLISIONDETECTION_HPP
#define COLLISIONDETECTION_HPP

#include <vector>
#include <memory>

class VisibleObject;

struct DeadPlayers
{
    std::shared_ptr<VisibleObject> player;
    double timeEnemyShouldLive;
};

class VisibleObject;
class ResourceManager;
class SoundPlayer;

class CollisionDetection
{
public:
    CollisionDetection();
/* returns true if collision happens between any player and enemy */
    bool DetectCollisions(unsigned int &lives, unsigned int &score,
                          std::vector<DeadPlayers> &deadVec, ResourceManager &resMan,
                          SoundPlayer &soundPlayer);

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
