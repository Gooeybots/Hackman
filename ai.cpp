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
    if(mEnemy && mPlayer)
    {
        Direction dir(Direction::None);

        std::list<std::shared_ptr<Positions>> closedList;
        std::deque<std::shared_ptr<Positions>> openList;
        std::shared_ptr<Positions> target(nullptr);
        std::shared_ptr<Positions> location(new Positions((int)mPlayer->GetX(),
                                                          (int)mPlayer->GetY(), 0, 0, nullptr));
        int x((int)mEnemy->GetX()), y((int)mEnemy->GetY());
        int distance(2);

        switch(mState)
        {
        case State::Chase:
            switch(mChaseType)
            {
            case ChaseType::Behind:
                distance = -distance;
            case ChaseType::Infront:
                switch(mEnemy->GetPrevDirection())
                {
                case Direction::Up:
                    y += distance; break;
                case Direction::Down:
                    y -= distance; break;
                case Direction::Left:
                    x -= distance; break;
                case Direction::Right:
                    x += distance; break;
                case Direction::None:
                default:
                    x += distance; y += distance; break;
                }
                break;
            case ChaseType::Stalk:
                distance = 5;
                switch(mEnemy->GetPrevDirection())
                {
                case Direction::Up:
                    x += distance; break;
                case Direction::Down:
                    x -= distance; break;
                case Direction::Left:
                    y -= distance; break;
                case Direction::Right:
                    y += distance; break;
                case Direction::None:
                default:
                    x += distance; y += distance; break;
                }
                break;
            case ChaseType::DeadOn:
            default:
                break;
            }
            break;
        case State::Patrol:
            SetXAndYLocation(x, y);
            if(WithinDistance(x, y, location->x, location->y, 2))
            {
                mPos = !mPos;
                SetXAndYLocation(x, y);
            }
            break;
        default:
            break;
        }
        target = std::make_shared<Positions>(x, y, 0, 0, nullptr);

        if(!(location->x == target->x && location->y == target->y))
        {
            location->distance = GetDistanceToTarget(location->x, location->y,
                                                     target->x, target->y);
            x = location->x; y = location->y;
            switch(mPlayer->GetPrevDirection())
            {
            case Direction::Up:
                y -= 1; break;
            case Direction::Down:
                y += 1; break;
            case Direction::Left:
                x += 1; break;
            case Direction::Right:
                x -= 1; break;
            case Direction::None:
            default:
                x -= 1; y -= 1;
                break;
            }
            std::shared_ptr<Positions> lastStep(new Positions(x, y, 1000, 0, nullptr));
            openList.push_back(location);
            closedList.push_back(lastStep);

            dir = FindShortestPath(target, location, closedList, openList, map);
        }
        else
            dir = mPlayer->GetPrevDirection();

        return dir;
    }
    return Direction::None;
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
            PutBaseMoveFromCurrentToPath(current, path);

        AddAdjacentSquares(openList, closedList, current, target, map);
        SortOpenListInOrderOfScore(openList);
    }while((!openList.empty()));

    if(!path)
        GetBestPathClosedList(closedList, path);

    return GetDirection(location, path);
}

void AI::SetXAndYLocation(int &x, int &y)
{
    x = (mPos ? mFirstPos.x : mSecondPos.x);
    y = (mPos ? mFirstPos.y : mSecondPos.y);
}

void AI::PutBaseMoveFromCurrentToPath(std::shared_ptr<Positions> &current,
                                      std::shared_ptr<Positions> &path)
{
    if(current->parent != nullptr)
    {
        while(current->parent != nullptr)
        {
            path = current;
            current = current->parent;
        }
    }
}

void AI::GetBestPathClosedList(std::list<std::shared_ptr<Positions> > &list,
                               std::shared_ptr<Positions> &path)
{
    if(!list.empty())
    {
        std::shared_ptr<Positions> current = list.back();
        for(auto positionPtr(list.begin()); positionPtr != list.end(); ++positionPtr)
        {
            if((*positionPtr)->distance < current->distance)
                current = *positionPtr;
            else if((*positionPtr)->distance == current->distance &&
                    (*positionPtr)->moves < current->moves)
                current = *positionPtr;
        }
        PutBaseMoveFromCurrentToPath(current, path);
        if(!path)
            path = list.back();
    }
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
                        found->moves = current->moves + 1;
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

bool AI::WithinDistance(int x, int y, int locationX, int locationY, const int distance)
{
    bool within(false);
    int lowerBoundX(locationX - distance), upperBoundX(locationX + distance),
            lowerBoundY(locationY - distance), upperBoundY(locationY + distance);

    if(x >= lowerBoundX && x <= upperBoundX && y >= lowerBoundY && y <= upperBoundY)
        within = true;

    return within;
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


void AI::SwitchStates()
{
    mState = mState == State::Patrol ? State::Chase : State::Patrol;
}
