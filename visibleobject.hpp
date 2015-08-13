#ifndef VISIBLEOBJECT_HPP
#define VISIBLEOBJECT_HPP

#include <glm/mat4x4.hpp>

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

class VisibleObject
{
public:
    VisibleObject(const float xOffset, const float yOffset, const unsigned int vao,
                  const unsigned int texture, const unsigned int program);
    virtual ~VisibleObject();
    virtual unsigned int GetPlayer(); // will return 0 if not a player
    void Draw(const glm::mat4 &View);

protected:
    float mXOffset, mYOffset;
    unsigned int mVao;
    unsigned int mTexture;
    unsigned int mProgram;
};

#endif // VISIBLEOBJECT_HPP
