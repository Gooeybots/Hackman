#include "setupobjects.hpp"
#include "resourcemanager.hpp"
#include "map.hpp"
#include "visibleobject.hpp"
#include "character.hpp"

std::shared_ptr<VisibleObject> GetObjectFromEnum(const Object obj, const float x,
                                                 const float y, ResourceManager &resMan);

void GetVecFullOfObjects(std::vector<std::shared_ptr<VisibleObject> > &vec,
                         const Map &map, ResourceManager &resMan)
{
    vec.clear();
    for(unsigned int y(0); y < 30; ++y)
    {
        for(unsigned int x(0); x < 28; ++x)
        {
            Object obj(map.GetWhichObject(glm::ivec2(x, y)));
            std::shared_ptr<VisibleObject> ptr(
                        GetObjectFromEnum(obj, x + 0.5f, y + 0.5f, resMan));
            if(ptr)
                vec.push_back(ptr);
        }
    }
}

std::shared_ptr<VisibleObject> GetObjectFromEnum(const Object obj, const float x,
                                                 const float y, ResourceManager &resMan)
{
    typedef std::shared_ptr<VisibleObject> ptr;
    float xOffset(x * 1.0f), yOffset(y * 1.0f);
    unsigned int geoffTexture(resMan.GetTexture("geoff.png")),
            sceneryTexture(resMan.GetTexture("scenery.png"));
    unsigned int vaoTopLeft(resMan.GetVao("vao top left")),
            vaoTopRight(resMan.GetVao("vao top right")),
            vaoBottomLeft(resMan.GetVao("vao bottom left")),
            vaoBottomRight(resMan.GetVao("vao bottom right"));
    unsigned int program(resMan.GetProgram("textured.vs", "textured.fs"));
    unsigned int characterProgram(resMan.GetProgram("character.vs", "textured.fs"));

    switch(obj)
    {
    case Object::player:
        return ptr(new Character(xOffset, yOffset, 4.0f, 1, vaoTopLeft, vaoBottomLeft,
                                 geoffTexture, characterProgram));
        break;
    case Object::enemy1:
        return ptr(new Character(xOffset, yOffset, 4.0f, 2, vaoTopLeft, vaoBottomLeft,
                                 geoffTexture, characterProgram));
        break;
    case Object::enemy2:
        return ptr(new Character(xOffset, yOffset, 4.0f, 4, vaoTopLeft, vaoBottomLeft,
                                 geoffTexture, characterProgram));
        break;
    case Object::enemy3:
        return ptr(new Character(xOffset, yOffset, 4.0f, 8, vaoTopLeft, vaoBottomLeft,
                                 geoffTexture, characterProgram));
        break;
    case Object::enemy4:
        return ptr(new Character(xOffset, yOffset, 4.0f, 16, vaoTopLeft, vaoBottomLeft,
                                 geoffTexture, characterProgram));
        break;
    case Object::tree:
        return ptr(new VisibleObject(xOffset, yOffset, vaoTopLeft, vaoBottomLeft,
                                     sceneryTexture, program));
        break;
    case Object::choppedTree:
        return ptr(new VisibleObject(xOffset, yOffset, vaoBottomLeft, vaoBottomLeft,
                                     sceneryTexture, program));
        break;
    case Object::block1:
        return ptr(new VisibleObject(xOffset, yOffset, vaoTopRight, vaoTopRight,
                                     sceneryTexture, program));
        break;
    case Object::block2: case Object::wolfEntrance:
        return ptr(new VisibleObject(xOffset, yOffset, vaoBottomRight, vaoBottomRight,
                                     sceneryTexture, program));
        break;
    default:
        break;
    }
    return nullptr;
}
