#include <algorithm>
#include "setupobjects.hpp"
#include "resourcemanager.hpp"
#include "map.hpp"
#include "visibleobject.hpp"
#include "character.hpp"
#include "ai.hpp"

bool ByPlayer(std::shared_ptr<VisibleObject> &a, std::shared_ptr<VisibleObject> &b);
std::shared_ptr<VisibleObject> GetObjectFromEnum(const Object obj, const float x,
                                                 const float y,  unsigned int &enemyNo,
                                                 ResourceManager &resMan);
std::shared_ptr<VisibleObject> GetPlayer(const unsigned int player,
                                         std::vector<std::shared_ptr<VisibleObject>> &objVec);

void GetVecFullOfObjects(std::vector<std::shared_ptr<VisibleObject> > &vec,
                         const Map &map, ResourceManager &resMan,
                         std::vector<AI> &aiVec)
{
    unsigned int enemy(2);
    vec.clear();
    for(unsigned int y(0); y < 30; ++y)
    {
        for(unsigned int x(0); x < 28; ++x)
        {
            Object obj(map.GetWhichObject(glm::ivec2(x, y)));
            std::shared_ptr<VisibleObject> ptr(
                        GetObjectFromEnum(obj, x + 0.5f, y + 0.5f, enemy, resMan));
            if(ptr)
                vec.push_back(ptr);
        }
    }
    std::sort(vec.begin(), vec.end(), ByPlayer);

    std::shared_ptr<VisibleObject> player(GetPlayer(1, vec));
    for(unsigned int i(0); i < enemy - 2; ++i)
    {
        float x(*(map.mAiPatrolPositions.begin() + (i * 4)));
        float y(*(map.mAiPatrolPositions.begin() + (i * 4) + 1));
        float x1(*(map.mAiPatrolPositions.begin() + (i * 4) + 2));
        float y1(*(map.mAiPatrolPositions.begin() + (i * 4) + 3));

        AI ai(GetPlayer(i + 2, vec), player, x, y, x1, y1);
        aiVec.push_back(ai);
    }
}

bool ByPlayer(std::shared_ptr<VisibleObject> &a,
              std::shared_ptr<VisibleObject> &b)
{
    bool retValue(false);
    if(a == b && b == 0)
        retValue = true;
    else if(a > b && b != 0)
        retValue = true;
    return retValue;
}

std::shared_ptr<VisibleObject> GetObjectFromEnum(const Object obj, const float x,
                                                 const float y, unsigned int &enemyNo,
                                                 ResourceManager &resMan)
{
    typedef std::shared_ptr<VisibleObject> ptr;
    float xOffset(x * 1.0f), yOffset(y * 1.0f);
    std::shared_ptr<unsigned int> geoffTexture(resMan.GetTexture("geoff.png")),
            sceneryTexture(resMan.GetTexture("scenery.png")),
            scenery2Texture(resMan.GetTexture("scenery2.png")),
            wolfTexture(resMan.GetTexture("wolf.png")),
            bearTexture(resMan.GetTexture("bear.png"));
    std::shared_ptr<unsigned int> vaoTopLeft(resMan.GetVao("vao top left")),
            vaoTopRight(resMan.GetVao("vao top right")),
            vaoBottomLeft(resMan.GetVao("vao bottom left")),
            vaoBottomRight(resMan.GetVao("vao bottom right"));
    std::shared_ptr<unsigned int> program(resMan.GetProgram("textured.vs", "textured.fs"));
    std::shared_ptr<unsigned int> characterProgram(resMan.GetProgram("character.vs", "textured.fs"));
    float speed(4.0f);

    switch(obj)
    {
    case Object::player:
        return ptr(new Character(xOffset, yOffset, speed, 1, vaoTopLeft, vaoBottomLeft,
                                 geoffTexture, characterProgram, 3));
        break;
    case Object::enemy1:
        enemyNo += 1;
        return ptr(new Character(xOffset, yOffset, speed, enemyNo - 1, vaoTopLeft, vaoBottomLeft,
                                 wolfTexture, characterProgram));
        break;
    case Object::enemy2:
        enemyNo += 1;
        return ptr(new Character(xOffset, yOffset, speed, enemyNo - 1, vaoTopLeft, vaoBottomLeft,
                                 bearTexture, characterProgram));
        break;
    case Object::enemy3:
        enemyNo += 1;
        return ptr(new Character(xOffset, yOffset, speed, enemyNo - 1, vaoTopLeft, vaoBottomLeft,
                                 geoffTexture, characterProgram));
        break;
    case Object::enemy4:
        enemyNo += 1;
        return ptr(new Character(xOffset, yOffset, speed, enemyNo - 1, vaoTopLeft, vaoBottomLeft,
                                 geoffTexture, characterProgram));
        break;
    case Object::tree:
        return ptr(new VisibleObject(xOffset, yOffset, vaoTopLeft, vaoBottomLeft,
                                     sceneryTexture, program));
        break;
    case Object::choppedTree:
        return ptr(new VisibleObject(xOffset, yOffset, vaoBottomLeft, nullptr,
                                     sceneryTexture, program));
        break;
    case Object::block1:
        return ptr(new VisibleObject(xOffset, yOffset, vaoTopRight, nullptr,
                                     sceneryTexture, program));
        break;
    case Object::block2:
        return ptr(new VisibleObject(xOffset, yOffset, vaoBottomRight, nullptr,
                                     sceneryTexture, program));
        break;
    case Object::wolfEntrance:
        return ptr(new VisibleObject(xOffset, yOffset, vaoBottomLeft, nullptr,
                                     scenery2Texture, program));
        break;
    case Object::powerPill:
        return ptr(new VisibleObject(xOffset, yOffset, vaoTopRight, nullptr,
                                     scenery2Texture, program));
        break;
    case Object::specialObject:
        return ptr(new VisibleObject(xOffset, yOffset, vaoBottomRight, nullptr,
                                     scenery2Texture, program));
        break;
    default:
        break;
    }
    return nullptr;
}

std::shared_ptr<VisibleObject> GetPlayer(const unsigned int player,
                                         std::vector<std::shared_ptr<VisibleObject> > &objVec)
{
    std::shared_ptr<VisibleObject> playerToRet(nullptr);
    for(auto it : objVec)
    {
        if(it->GetPlayer() == player)
        {
            playerToRet = it;
            break;
        }
    }
    return playerToRet;
}
