#include <GL/glew.h>
#include "map.hpp"
#include "character.hpp"

Character::Character(const float x, const float y, const float speed,
                     const unsigned int player, const unsigned int vao,
                     const unsigned int nextVao, const unsigned int texture,
                     const unsigned int program, Direction dir):
    VisibleObject(x, y, vao, nextVao, texture, program),
    prevDir(dir), mSpeed(speed), mPlayer(player)
{}

Character::~Character(){}

bool Character::Move(const Direction dir, const float dt, const Map &map)
{
    bool moved(false);
    glm::vec2 centerOfChar(mXOffset + 0.5f, mYOffset + 0.5f);

    float distanceToMove((dir == Direction::Down || dir == Direction::Left) ?
                             -(mSpeed * dt) : mSpeed * dt);

    if(!CanMoveWithoutChangingSquare(centerOfChar, distanceToMove, dir))
    {
        if(map.CanMove(dir, glm::ivec2((int)centerOfChar.x,
                                       (int)centerOfChar.y)))
        {
            prevDir = dir;
            if(dir == Direction::Left || dir == Direction::Right)
            {
                int oldValue((int)centerOfChar.x);
                mXOffset += distanceToMove;
                if(mXOffset + 0.5 < oldValue || mXOffset + 0.5 >= oldValue)
                    moved = true;
            }
            else
            {
                int oldValue((int)centerOfChar.y);
                mYOffset += distanceToMove;
                if(mYOffset + 0.5 < oldValue || mYOffset + 0.5 >= oldValue)
                    moved = true;
            }
        }
        else
        {
            if(dir != prevDir)
                moved = Move(prevDir, dt, map);
        }
    }
    else
    {
        centerOfChar = glm::vec2(mXOffset + 0.5f, mYOffset + 0.5f);
        if(!map.CanMove(dir, glm::ivec2((int)centerOfChar.x,
                                        (int)centerOfChar.y)))
        {
            MoveToCenter(dir);
        }
    }
    return moved;
}

unsigned int Character::GetPlayer()
{
    return mPlayer;
}

bool Character::CanMoveWithoutChangingSquare(const glm::vec2 &where,
                                             const float distanceToMove,
                                             const Direction dir)
{
    bool canMove(false);
    glm::vec2 newPos(where);
    if(dir == Direction::Down || dir == Direction::Up)
    {
        newPos.y += distanceToMove;
        if((int)newPos.y == (int)where.y)
        {
            mYOffset += distanceToMove;
            canMove = true;
        }
    }
    else
    {
        newPos.x += distanceToMove;
        if((int)newPos.x == (int)where.x)
        {
            mXOffset += distanceToMove;
            canMove = true;
        }
    }
    return canMove;
}

void Character::MoveToCenter(const Direction dir)
{
    glm::ivec2 whereInt((int)(mXOffset + 0.5f), (int)(mYOffset + 0.5f));
    glm::vec2 whereFromCenter((mXOffset + 0.5f) - whereInt.x,
                              (mYOffset + 0.5f) - whereInt.y);

    if((dir == Direction::Up && whereFromCenter.y > 0.5)
            || (dir == Direction::Down && whereFromCenter.y < 0.5))
        mYOffset = whereInt.y * 1.0f;
    else if((dir == Direction::Left && whereFromCenter.x < 0.5)
            || (dir == Direction::Right && whereFromCenter.x > 0.5))
        mXOffset = whereInt.x * 1.0f;
}
