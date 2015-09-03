#include <string>
#include <sstream>
#include "map.hpp"
#include "readtostream.hpp"
#include "visibleobject.hpp"
#include "character.hpp"

Map::Map(const char * mapsFilename):mTrees(0)
{
    mMap.fill(0);

    std::stringstream data;
    if(ReadToStream(mapsFilename, data))
    {
        std::string str;
        while(data >> str)
            mMapFiles.push_back(str.c_str());

        mWhichMap = mMapFiles.begin();

        RetriveMapFromFile();
    }
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

bool Map::CanMove(const Direction dir, const glm::ivec2 &where) const
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
        return CanPassThroughObject(obj);
    }
    return false;
}

bool Map::RetriveMapFromFile()
{
    std::stringstream data;
    if(ReadToStream(*mWhichMap, data))
    {
        mWhichMap++;
        for(auto & square : mMap)
        {
            unsigned int item;
            data >> item;
            square = item;
            if(item == 1)
                mTrees += 1;
        }
        return true;
    }
    return false;
}

void Map::SetObject(const glm::ivec2 &whichSquare, const unsigned int player)
{
    unsigned int where(whichSquare.x + (whichSquare.y * 28));
    Object obj(GetWhichObject(whichSquare));

    if(obj == Object::player || obj == Object::enemy1 || obj == Object::enemy2 ||
            obj == Object::enemy3 || obj == Object::enemy4)
        mMap[where] += player;
    else if(obj == Object::tree)
    {
        mMap[where] = player;
        mTrees -= 1;
    }
    else
        mMap[where] = player;
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

bool Map::CanPassThroughObject(const Object obj) const
{
    bool retValue(false);
    switch(obj)
    {
    case Object::block1: case Object::block2: case Object::wolfEntrance:
        retValue = false;
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
