#include <algorithm>
#include <glm/vec2.hpp>
#include "visibleobject.hpp"
#include "directionenum.hpp"
#include "map.hpp"
#include "ai.hpp"

bool SortPositions(std::shared_ptr<AI::Positions> &a,
                   std::shared_ptr<AI::Positions> &b);

Direction AI::GetMove(Map &map)
{
    std::shared_ptr<Positions> target(new Positions(
                (int)mEnemy->GetX(), (int)mEnemy->GetY(), 0, 0, nullptr));
    std::shared_ptr<Positions> location(new Positions(
                (int)mPlayer->GetX(), (int)mPlayer->GetY(), 0, 0, nullptr));
    std::list<std::shared_ptr<Positions>> closedList;
    std::deque<std::shared_ptr<Positions>> openList;

    openList.push_back(location);
    return FindShortestPath(target, location, closedList, openList, map);
}

Direction AI::FindShortestPath(std::shared_ptr<Positions> &target,
                               std::shared_ptr<Positions> &location,
                               std::list<std::shared_ptr<Positions>> &closedList,
                               std::deque<std::shared_ptr<Positions>> &openList,
                               Map &map)
{
    std::shared_ptr<Positions> path(nullptr), current(location);
    do
    {
        if(!openList.empty())
            current = openList.front();

        AddToClosedList(closedList, current);
        RemoveFromOpenList(openList, current);

        if(current->x == target->x && current->y == target->y)
        {
            while(current->parent != nullptr)
            {
                path = current;
                current = current->parent;
            }

            break;
        }
        AddAdjacentSquares(openList, closedList, current, target, map);
        SortOpenListInOrderOfScore(openList);
    }while((!openList.empty()));

    if(!path)
        return Direction::None;
    else
        return GetDirection(location, path);
}

unsigned int AI::GetPlayer()
{
    return mPlayer->GetPlayer();
}

void AI::AddAdjacentSquares(std::deque<std::shared_ptr<Positions> > &openList,
                            const std::list<std::shared_ptr<Positions> > &closedList,
                            const std::shared_ptr<Positions> &current,
                            const std::shared_ptr<Positions> &target,
                            Map &map)
{
    std::vector<glm::ivec2> moves;
    glm::ivec2 left(current->x - 1, current->y),
            right(current->x + 1, current->y),
            up(current->x, current->y + 1),
            down(current->x, current->y - 1);

    moves.push_back(left); moves.push_back(right);
    moves.push_back(up); moves.push_back(down);

    for(auto & possMoves : moves)
    {
        if(CanMove(possMoves.x, possMoves.y,
                   map.GetWhichObject(possMoves)))
        {
            if(!IsInClosedList(possMoves.x, possMoves.y, closedList))
            {
                std::shared_ptr<Positions> found(
                            CheckOpenListForSquare(possMoves.x, possMoves.y, openList));
                if(!found)
                {
                    AddToOpenList(openList, possMoves.x, possMoves.y,
                                  current->moves + 1,
                                  GetDistanceToTarget(possMoves.x, possMoves.y, target->x, target->y),
                                  current);
                }
                else
                {
                    if(found->moves > current->moves + 1)
                    {
                        found->parent = current;
                        found->moves = current->moves +1;
                    }
                }
            }
        }
    }
    moves.clear();
}

void AI::AddToOpenList(std::deque<std::shared_ptr<Positions> > &openList,
                       const unsigned int x, const unsigned int y,
                       const unsigned int movesTaken,
                       const unsigned int distanceToTarget,
                       const std::shared_ptr<Positions> &current)
{
    openList.push_back(
                std::shared_ptr<Positions>(new Positions(x, y, distanceToTarget, movesTaken, current)));
}

void AI::AddToClosedList(std::list<std::shared_ptr<Positions> > &closedList,
                         std::shared_ptr<Positions> &current)
{
    closedList.push_back(current);
}

void AI::RemoveFromOpenList(std::deque<std::shared_ptr<Positions> > &openList,
                            std::shared_ptr<Positions> &current)
{
    for(auto where(openList.begin()); where != openList.end(); ++where)
    {
        if((*where) == current)
        {
            openList.erase(where);
            break;
        }
    }
}

void AI::SortOpenListInOrderOfScore(
        std::deque<std::shared_ptr<Positions> > &openList)
{
    std::sort(openList.begin(), openList.end(), SortPositions);
}

bool SortPositions(std::shared_ptr<AI::Positions> &a,
                   std::shared_ptr<AI::Positions> &b)
{
    return ((a->moves + a->distance) < (b->moves + b->distance));
}

bool AI::CanMove(const int x, const int y, const Object nextSquare)
{
    if(((x < 0 || x > 27 || y > 29 || y < 0)
        && nextSquare != Object::passThrough) ||
            nextSquare == Object::block1 || nextSquare == Object::block2)
        return false;
    return true;
}

bool AI::IsInClosedList(const unsigned int x, const unsigned int y,
                        const std::list<std::shared_ptr<Positions> > &closedList)
{
    bool found(false);
    if(!closedList.empty())
    {
        for(auto & position : closedList)
        {
            if(position->x == x && position->y == y)
            {
                found = true;
                break;
            }
        }
    }
    return found;
}

Direction AI::GetDirection(const std::shared_ptr<Positions> &start,
                           const std::shared_ptr<Positions> &end)
{
    Direction dir(Direction::None);
    if(end->x > start->x)
        dir = Direction::Right;
    else if(end->x < start->x)
        dir = Direction::Left;
    else if(end->y > start->y)
        dir = Direction::Up;
    else if(end->y < start->y)
        dir = Direction::Down;
    return dir;
}

unsigned int AI::GetDistanceToTarget(const unsigned int x,
                                     const unsigned int y,
                                     const unsigned int targetX,
                                     const unsigned int targetY)
{
    unsigned int distance(0);

    distance = (x >= targetX ? x - targetX : targetX - x);
    distance += (y >= targetY ? y - targetY : targetY - y);

    return distance;
}

std::shared_ptr<AI::Positions> AI::CheckOpenListForSquare(const unsigned int x, const unsigned int y,
                                                      const std::deque<std::shared_ptr<Positions> > &openList)
{
    std::shared_ptr<Positions> found(nullptr);
    for(auto it(openList.begin()); it != openList.end(); ++it)
    {
        if((*it)->x == x && (*it)->y == y)
        {
            found = *it;
            break;
        }
    }
    return found;
}
