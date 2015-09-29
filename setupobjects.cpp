#include <algorithm>
#include <memory>
#include "setupobjects.hpp"
#include "resourcemanager.hpp"
#include "map.hpp"
#include "visibleobject.hpp"
#include "character.hpp"
#include "ai.hpp"
#include "mapeditor.hpp"
#include "enemy.hpp"

void UpdateSpecialObj(std::vector<std::shared_ptr<VisibleObject>> &vec, ResourceManager &resMan);
bool ByPlayer(std::shared_ptr<VisibleObject> &a, std::shared_ptr<VisibleObject> &b);
void FromMapToVec(std::vector<std::shared_ptr<VisibleObject>> &vec,
                  const Map &map, ResourceManager &resMan, unsigned int &enemyno);
std::shared_ptr<VisibleObject> GetObjectFromEnum(const Object obj, const float x,
                                                 const float y,  unsigned int &enemyNo,
                                                 ResourceManager &resMan);

bool GetVecFullOfObjects(std::vector<std::shared_ptr<VisibleObject> > &vec, Map &map,
                         ResourceManager &resMan, std::vector<PatrolArea> &patVec)
{
    patVec.clear();
    unsigned int enemy(2);
    FromMapToVec(vec, map, resMan, enemy);

    for(unsigned int i(0); i < enemy - 2; ++i)
    {
        float x(*(map.mAiPatrolPositions.begin() + (i * 4)));
        float y(*(map.mAiPatrolPositions.begin() + (i * 4) + 1));
        float x1(*(map.mAiPatrolPositions.begin() + (i * 4) + 2));
        float y1(*(map.mAiPatrolPositions.begin() + (i * 4) + 3));

        patVec.push_back(PatrolArea(i + 2, x, y, x1, y1));
    }
    map.mAiPatrolPositions.clear();
    return true;
}

bool GetVecFullOfObjects(std::vector<std::shared_ptr<VisibleObject> > &vec, Map &map,
                         ResourceManager &resMan, std::vector<std::shared_ptr<AI>> &aiVec)
{
    aiVec.clear();
    unsigned int enemy(2);
    FromMapToVec(vec, map, resMan, enemy);
    UpdateSpecialObj(vec, resMan);
    std::shared_ptr<VisibleObject> player(GetPlayer(1, vec));
    if(player)
    {
        for(unsigned int i(0); i < enemy - 2; ++i)
        {
            std::shared_ptr<VisibleObject> enemy(GetPlayer(i + 2, vec));
            Object obj(map.GetWhichObject(glm::ivec2((int)enemy->GetX(), (int)enemy->GetY())));
            AI::ChaseType chase;

            switch(obj)
            {
            case Object::enemy2:
                chase = AI::ChaseType::Infront;
                break;
            case Object::enemy3:
                chase = AI::ChaseType::Behind;
                break;
            case Object::enemy4:
                chase = AI::ChaseType::Stalk;
                break;
            default:
                chase = AI::ChaseType::DeadOn;
            }

            float x(*(map.mAiPatrolPositions.begin() + (i * 4)));
            float y(*(map.mAiPatrolPositions.begin() + (i * 4) + 1));
            float x1(*(map.mAiPatrolPositions.begin() + (i * 4) + 2));
            float y1(*(map.mAiPatrolPositions.begin() + (i * 4) + 3));

            aiVec.push_back(std::make_shared<AI>(enemy, player, x, y, x1, y1, chase));
        }
        map.mAiPatrolPositions.clear();
        std::sort(vec.begin(), vec.end(), ByPlayer);

        return true;
    }
    return false;
}

void FromMapToVec(std::vector<std::shared_ptr<VisibleObject> > &vec,
                  const Map &map, ResourceManager &resMan, unsigned int &enemy)
{
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
            bearTexture(resMan.GetTexture("bear.png")),
            snakeTexture(resMan.GetTexture("snake.png"));
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
                                 geoffTexture, characterProgram, vaoBottomRight));
        break;
    case Object::enemy1:
        enemyNo += 1;
        return ptr(new Enemy(xOffset, yOffset, speed, enemyNo - 1, vaoTopLeft, vaoBottomLeft,
                             wolfTexture, characterProgram, vaoBottomRight, vaoTopRight));
        break;
    case Object::enemy2:
        enemyNo += 1;
        return ptr(new Enemy(xOffset, yOffset, speed, enemyNo - 1, vaoTopLeft, vaoBottomLeft,
                             bearTexture, characterProgram, vaoBottomRight, vaoTopRight));
        break;
    case Object::enemy3:
        enemyNo += 1;
        return ptr(new Enemy(xOffset, yOffset, speed, enemyNo - 1, vaoTopLeft, vaoBottomLeft,
                             geoffTexture, characterProgram, vaoBottomRight, vaoTopRight));
        break;
    case Object::enemy4:
        enemyNo += 1;
        return ptr(new Enemy(xOffset, yOffset, speed, enemyNo - 1, vaoTopLeft, vaoBottomLeft,
                             snakeTexture, characterProgram, vaoBottomRight, vaoTopRight));
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

void UpdateSpecialObj(std::vector<std::shared_ptr<VisibleObject> > &vec, ResourceManager &resMan)
{
    std::shared_ptr<unsigned int> vao(resMan.GetVao("vao bottom right")),
            texture(resMan.GetTexture("scenery2.png"));

    for(auto & obj : vec)
    {
        if(*(obj->GetTexture()) == *texture && *(obj->GetVao()) == *vao)
        {
            obj->SwitchVaos();
            break;
        }
    }
}
