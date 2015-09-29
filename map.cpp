#include <string>
#include <sstream>
#include "map.hpp"
#include "readtostream.hpp"
#include "visibleobject.hpp"
#include "character.hpp"

Map::Map():mTrees(0)
{
    mMap.fill(0);

    std::stringstream data;
    if(ReadToStream("maps.txt", data))
    {
        std::string str;
        while(data >> str)
        {
            mMapFiles.push_back(str);
        }

        mWhichMap = mMapFiles.begin();
    }

}

Map::Map(const char * filename):mTrees(0)
{
    mMap.fill(0);
    mMapFiles.push_back(filename);
    mWhichMap = mMapFiles.begin();
    RetriveMapFromFile();
}

bool Map::HasFinished()
{
    if(mTrees == 0)
        return true;
    return false;
}

bool Map::LoadNextMap()
{
    if(mWhichMap != mMapFiles.end())
    {
        if(RetriveMapFromFile())
            return true;
    }

    return false;
}

bool Map::CanMove(const Direction dir, const glm::ivec2 &where, const bool active) const
{
    if(CanTravelDirection(where.x, where.y, dir))
    {
        glm::ivec2 nextSquare(where.x, where.y);
        if(dir == Direction::Left)
            nextSquare.x -= 1;
        else if(dir == Direction::Right)
            nextSquare.x += 1;
        else if(dir == Direction::Up)
            nextSquare.y += 1;
        else if(dir == Direction::Down)
            nextSquare.y -= 1;
        Object obj(GetWhichObject(nextSquare));
        return CanPassThroughObject(obj, active);
    }
    return false;
}

bool Map::RetriveMapFromFile()
{
    mTrees = 0;
    std::stringstream data;
    if(ReadToStream((*mWhichMap).c_str(), data))
    {
        mWhichMap++;
        if(mWhichMap == mMapFiles.end())
            mWhichMap = mMapFiles.begin(); // so it loops through the maps

        for(auto & square : mMap)
        {
            unsigned int item(0);
            data >> item;
            square = item;
            if(item == 1)
                mTrees += 1;
            else if(item == 10 || item == 11 || item == 12 || item == 13)
            {
                float x(0.0f), y(0.0f), x1(0.0f), y1(0.0f);
                data >> x >> y >> x1 >> y1;
                mAiPatrolPositions.push_back(x);
                mAiPatrolPositions.push_back(y);
                mAiPatrolPositions.push_back(x1);
                mAiPatrolPositions.push_back(y1);
            }
        }
        return true;
    }
    return false;
}

bool Map::UpdateSpecialObj(const glm::ivec2 &whichSquare)
{
    bool retValue(false);
    unsigned int where(whichSquare.x + (whichSquare.y * 28));

    if(mMap[where] != 8)
    {
        mMap[where] = 8;
        retValue = true;
    }
    return retValue;
}

void Map::HideSpecialObj()
{
    for(auto & where : mMap)
    {
        if(where == 8)
        {
            where = 0;
            break;
        }
    }
}

void Map::SetObject(const glm::ivec2 &whichSquare)
{
    unsigned int where(whichSquare.x + (whichSquare.y * 28));
    Object obj(GetWhichObject(whichSquare));

    if(obj == Object::tree)
    {
        mMap[where] = 2;
        mTrees -= 1;
    }
    else
        mMap[where] = 0;
}

bool Map::CanTravelDirection(const unsigned int x, const unsigned int y,
                             const Direction dir) const
{
    if((dir == Direction::Left && x == 0) ||
            (dir == Direction::Right && x == 27) ||
            (dir == Direction::Up && y == 29) ||
            (dir == Direction::Down && y == 0))
        return false;
    return true;
}

bool Map::CanPassThroughObject(const Object obj, const bool active) const
{
    bool retValue(false);
    switch(obj)
    {
    case Object::block1: case Object::block2:
        retValue = false;
        break;
    case Object::wolfEntrance:
        retValue = active;
        break;
    default:
        retValue = true;
    }

    return retValue;
}

std::array<unsigned int, 840> Map::GetMap()
{
    return mMap;
}

Object Map::GetWhichObject(const glm::ivec2 &whichSquare) const
{
    Object obj;
    unsigned int where((whichSquare.y * 28) + whichSquare.x);
    unsigned int item(mMap[where]);
    switch(item)
    {
    case 1:
        obj = Object::tree;
        break;
    case 2:
        obj = Object::choppedTree;
        break;
    case 3:
        obj = Object::block1;
        break;
    case 4:
        obj = Object::block2;
        break;
    case 5:
        obj = Object::wolfEntrance;
        break;
    case 6:
        obj = Object::passThrough;
        break;
    case 7:
        obj = Object::powerPill;
        break;
    case 8:
        obj = Object::specialObject;
        break;
    case 9:
        obj = Object::player;
        break;
    case 10:
        obj = Object::enemy1;
        break;
    case 11:
        obj = Object::enemy2;
        break;
    case 12:
        obj = Object::enemy3;
        break;
    case 13:
        obj = Object::enemy4;
        break;
    default:
        obj = Object::none;
        break;
    }
    return obj;
}
