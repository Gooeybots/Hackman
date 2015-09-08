#ifndef AI_HPP
#define AI_HPP

#include <array>
#include <memory>
#include <deque> // open list
#include <list> //   closed list
#include <glm/vec2.hpp>
#include "directionenum.hpp"

class Map;
class VisibleObject;

class AI
{
    public:
    class Positions
    {
    public:
        Positions(const unsigned int xIn, const unsigned int yIn,
                  const unsigned int distanceIn, const unsigned int movesIn,
                  const std::shared_ptr<Positions> parentIn):
            x(xIn), y(yIn), distance(distanceIn), moves(movesIn), parent(parentIn)
        {}

        unsigned int x;
        unsigned int y;
        unsigned int distance;
        unsigned int moves;
        std::shared_ptr<Positions> parent;
    };

    enum State
    {
        Chase,
        Retreat,
        Patrol
    };
    enum ChaseType
    {
        DeadOn,
        Infront,
        Behind,
        Stalk
    };

    AI(std::shared_ptr<VisibleObject> player,
       std::shared_ptr<VisibleObject> enemy,
       const float pos1X, const float pos1Y,
       const float pos2X, const float pos2Y):
        mFirstPos(pos1X, pos1Y), mSecondPos(pos2X, pos2Y),
        mPlayer(player), mEnemy(enemy), mPos(true)
    {
        mState = State::Patrol;
        mChaseType = ChaseType::Stalk;
    }

    Direction GetMove(Map &map);
    unsigned int GetPlayer();
    glm::vec2 mFirstPos;
    glm::vec2 mSecondPos;
private:
    Direction FindShortestPath(std::shared_ptr<Positions> &target,
                          std::shared_ptr<Positions> &location,
                          std::list<std::shared_ptr<Positions>> &closedList,
                          std::deque<std::shared_ptr<Positions>> &openList,
                          Map &map);

    void SetXAndYLocation(int &x, int &y);
    void PutBaseMoveFromCurrentToPath(std::shared_ptr<Positions> &current,
                                      std::shared_ptr<Positions> &path);

    void GetBestPathClosedList(std::list<std::shared_ptr<Positions>> &list,
                               std::shared_ptr<Positions> &path);

    void AddAdjacentSquares(std::deque<std::shared_ptr<Positions>> &openList,
                            const std::list<std::shared_ptr<Positions>> &closedList,
                            const std::shared_ptr<Positions> &current,
                            const std::shared_ptr<Positions> &target,
                            Map &map);
    void AddToOpenList(std::deque<std::shared_ptr<Positions>> &openList,
                       const unsigned int x, const unsigned int y,
                       const unsigned int movesTaken,
                       const unsigned int distanceToTarget,
                       const std::shared_ptr<Positions> &current);
    void AddToClosedList(std::list<std::shared_ptr<Positions>> &closedList,
                         std::shared_ptr<Positions> &current);
    void RemoveFromOpenList(std::deque<std::shared_ptr<Positions>> &openList,
                            std::shared_ptr<Positions> &current);
    void SortOpenListInOrderOfScore(std::deque<std::shared_ptr<Positions>> &openList);
    bool WithinDistance(int x, int y, int locationX, int locationY, const int distance);
    bool CanMove(const int x, const int y, const Object nextSquare);
    bool IsInClosedList(const unsigned int x, const unsigned int y,
                        const std::list<std::shared_ptr<Positions>> &closedList);
    Direction GetDirection(const std::shared_ptr<Positions> &start,
                           const std::shared_ptr<Positions> &end);
    unsigned int GetDistanceToTarget(const unsigned int x,
                                     const unsigned int y,
                                     const unsigned int targetX,
                                     const unsigned int targetY);
    std::shared_ptr<Positions>
        CheckOpenListForSquare(const unsigned int x, const unsigned int y,
                               const std::deque<std::shared_ptr<Positions>> &openList);

    std::shared_ptr<VisibleObject> mPlayer;
    std::shared_ptr<VisibleObject> mEnemy;
    State mState;
    ChaseType mChaseType;
    bool mPos;
};

#endif // AI_HPP
