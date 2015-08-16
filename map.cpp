#include <string>
#include <sstream>
#include "map.hpp"
#include "readtostream.hpp"
#include "visibleobject.hpp"
#include "character.hpp"

Map::Map(const char * mapsFilename):mTrees(0)
{
    for(unsigned int y(0); y < 30; ++y)
    {
        for(unsigned int x(0); x < 28; ++x)
        {
            mMap[x][y] = 0;
        }
    }

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
        for(unsigned int y(0); y < 30; ++y)
        {
            for(unsigned int x(0); x < 28; ++x)
            {
                unsigned int item;
                data >> item;
                mMap[x][y] = item;
                if(item == 32)
                    mTrees += 1;
            }
        }
        return true;
    }
    return false;
}

void Map::SetObject(const glm::ivec2 &whichSquare, const unsigned int player)
{
    Object obj(GetWhichObject(whichSquare));
    if(obj == Object::player || obj == Object::enemy1 || obj == Object::enemy2 ||
            obj == Object::enemy3 || obj == Object::enemy4)
    {
        mMap[whichSquare.x][whichSquare.y] += player;
    }
    else if(obj == Object::tree)
    {
        mMap[whichSquare.x][whichSquare.y] = player;
        mTrees -= 1;
    }
    else
        mMap[whichSquare.x][whichSquare.y] = player;
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

Object Map::GetWhichObject(const glm::ivec2 &whichSquare) const
{
    Object obj;
    unsigned int item(mMap[whichSquare.x][whichSquare.y]);
    switch(item)
    {
    case 1:
        obj = Object::player;
        break;
    case 2: case 3:
        obj = Object::enemy1;
        break;
    case 4: case 5: case 6: case 7:
        obj = Object::enemy2;
        break;
    case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
        obj = Object::enemy3;
        break;
    case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23:
    case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31:
        obj = Object::enemy4;
        break;
    case 32:
        obj = Object::tree;
        break;
    case 33:
        obj = Object::choppedTree;
        break;
    case 34:
        obj = Object::block1;
        break;
    case 35:
        obj = Object::block2;
        break;
    case 36:
        obj = Object::wolfEntrance;
        break;
    case 37:
        obj = Object::passThrough;
        break;
    case 38:
        obj = Object::powerPill;
        break;
    case 39:
        obj = Object::specialObject;
        break;
    default:
        obj = Object::none;
        break;
    }
    return obj;
}
