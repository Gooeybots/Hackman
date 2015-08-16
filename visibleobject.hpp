#ifndef VISIBLEOBJECT_HPP
#define VISIBLEOBJECT_HPP

#include <glm/mat4x4.hpp>

class Map;

enum Direction
{
    None,
    Up,
    Down,
    Left,
    Right
};

class VisibleObject
{
public:
    VisibleObject(const float xOffset, const float yOffset, const unsigned int vao,
                  const unsigned int nextVao, const unsigned int texture,
                  const unsigned int program);
    virtual ~VisibleObject();

    virtual void Draw(const glm::mat4 &view);
    virtual bool Move(const Direction dir, const float dt, const Map &map);
    virtual unsigned int GetPlayer(); // will return 0 if not a player
    virtual glm::mat4 GetModel();

    void SwitchVaos();
    float GetX();
    float GetY();

protected:
    float mXOffset, mYOffset;
    unsigned int mVao, mNextVao;
    unsigned int mTexture;
    unsigned int mProgram;
};

#endif // VISIBLEOBJECT_HPP
