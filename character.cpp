#include <GL/glew.h>
#include "character.hpp"

Character::Character(const float x, const float y, const float speed,
                     const unsigned int player, const unsigned int vao,
                     const unsigned int texture, const unsigned int program):
    mSpeed(speed), mPlayer(player),
    VisibleObject(x, y, vao, texture, program)
{}

Character::~Character(){}

bool Character::Move(const Direction dir, const float dt)
{
    bool moved(false);
    float distanceToMove((dir == Direction::Down || dir == Direction::Left) ?
                              -(mSpeed * dt) : mSpeed * dt);

    if(dir == Direction::Left || dir == Direction::Right)
    {
        int oldValue((int)mXOffset);
        mXOffset += distanceToMove;
        if(mXOffset < oldValue || mXOffset > oldValue + 1)
            moved = true;
    }
    else
    {
        int oldValue((int)mYOffset);
        mYOffset += distanceToMove;
        if(mYOffset < oldValue || mYOffset > oldValue + 1)
            moved = true;
    }
    return moved;
}

unsigned int Character::GetPlayer()
{
    return mPlayer;
}
