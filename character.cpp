#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "map.hpp"
#include "character.hpp"

Character::Character(const float x, const float y, const float speed,
                     const unsigned int player, const unsigned int vao,
                     const unsigned int nextVao, const unsigned int texture,
                     const unsigned int program, Direction dir):
    VisibleObject(x, y, vao, nextVao, texture, program),
    prevDir(dir), currTime(0.0), prevTime(0.0f), mSpeed(speed),
    mPlayer(player), mModel(1.0f){}

Character::~Character(){}

glm::mat4 Character::GetModel()
{
    return mModel;
}

void Character::Draw(const glm::mat4 &view)
{
    UpdateModel(prevDir);
    glUseProgram(mProgram);

    glBindVertexArray(mVao);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glUniformMatrix4fv(glGetUniformLocation(mProgram, "modelView"),
                       1, GL_FALSE, glm::value_ptr(view * mModel));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

bool Character::Move(const Direction dir, const float dt, const Map &map)
{
    // Update animation
    currTime = glfwGetTime();
    if(currTime - prevTime >= 0.2) // should probably change from magic no
    {
        SwitchVaos();
        prevTime = currTime;
    }

    bool moved(false);
    float halfway(0.5f);
    float distanceToMove(mSpeed * dt);
    if(dir == Direction::None)
    {
        if(prevDir == Direction::None) // Shouldn't ever be the case
            return false;
        moved = Move(prevDir, dt, map); // Try going the same way as before
    }
    else if(!CanMoveWithoutChangingSquare(glm::vec2(mXOffset, mYOffset),
                                          distanceToMove, dir))
    { 
        if(map.CanMove(dir, glm::ivec2((int)mXOffset, (int)mYOffset)))
        {
            prevDir = dir;
            if(dir == Direction::Left || dir == Direction::Right)
            {
                if(mYOffset - (int)mYOffset != halfway)
                    MoveToCenterVertical(Direction::Up, distanceToMove);

                int oldValue((int)mXOffset);
                mXOffset += (dir == Direction::Left ? -distanceToMove : distanceToMove);
                if(mXOffset < oldValue || mXOffset >= oldValue)
                    moved = true;
            }
            else
            {
                if(mXOffset - (int)mXOffset != halfway)
                    MoveToCenterHorizontal(Direction::Left, distanceToMove);

                int oldValue((int)mYOffset);
                mYOffset += (dir == Direction::Down ? -distanceToMove : distanceToMove);
                if(mYOffset < oldValue || mYOffset >= oldValue)
                    moved = true;
            }
        }
        else
        {
            MoveToCenter(dir, distanceToMove);
            if(dir != prevDir) // if Geoff cant move try previous direction
                moved = Move(prevDir, dt, map);
            else // if not moving stop animation
                prevTime = currTime;
        }
    }
    else
    {
        if(!map.CanMove(dir, glm::ivec2((int)mXOffset, (int)mYOffset)))
        {
            MoveToCenter(dir, distanceToMove);
// To keep Geoff moving if user presses direction which he cannot move
            if(dir != prevDir)
                moved = Move(prevDir, dt, map);
            else // if it is moving stop animation
                prevTime = currTime;
        }
        else
        {
            if(dir == Direction::Left || dir == Direction::Right)
            {
                if(mYOffset - (int)mYOffset != halfway)
                    MoveToCenterVertical(Direction::Up, distanceToMove);

                mXOffset += (dir == Direction::Left ? -distanceToMove : distanceToMove);
            }
            else
            {
                if(mXOffset - (int)mXOffset != halfway)
                    MoveToCenterHorizontal(Direction::Left, distanceToMove);

                mYOffset += (dir == Direction::Down ? -distanceToMove : distanceToMove);
            }
            prevDir = dir;
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
        newPos.y += (dir == Direction::Down ? -distanceToMove : distanceToMove);
        if((int)newPos.y == (int)where.y)
            canMove = true;
    }
    else
    {
        newPos.x += (dir == Direction::Left ? -distanceToMove : distanceToMove);
        if((int)newPos.x == (int)where.x)
            canMove = true;
    }
    return canMove;
}

void Character::MoveToCenter(const Direction dir,
                             const float distanceToMove)
{
    if(dir == Direction::Up || dir == Direction::Down)
    {
        MoveToCenterVertical(dir, distanceToMove);
    }
    else
        MoveToCenterHorizontal(dir, distanceToMove);
}

void Character::MoveToCenterHorizontal(const Direction dir,
                                       const float distanceToMove)
{
    float halfway(0.5f);
    float whereCenter(mXOffset - (int)mXOffset);

    if(dir == Direction::Right)
    {
        if(whereCenter > halfway && whereCenter - distanceToMove > halfway)
            mXOffset -= distanceToMove;
        else if(whereCenter > halfway && whereCenter - distanceToMove < halfway)
            mXOffset = (int)mXOffset + halfway;
        else if(whereCenter < halfway && whereCenter + distanceToMove < halfway)
            mXOffset += distanceToMove;
    }
    else
    {
        if(whereCenter < halfway && whereCenter + distanceToMove < halfway)
            mXOffset += distanceToMove;
        else if(whereCenter < halfway && whereCenter + distanceToMove > halfway)
            mXOffset = (int)mXOffset + halfway;
        else if(whereCenter > halfway && whereCenter - distanceToMove > halfway)
            mXOffset -= distanceToMove;
    }
}

void Character::MoveToCenterVertical(const Direction dir,
                                     const float distanceToMove)
{
    float halfway(0.5f);
    float whereCenter(mYOffset - (int)mYOffset);

    if(dir == Direction::Up)
    {
        if(whereCenter > halfway && whereCenter - distanceToMove > halfway)
            mYOffset -= distanceToMove;
        else if(whereCenter < halfway && whereCenter + distanceToMove > halfway)
            mYOffset = (int)mYOffset + halfway;
        else if(whereCenter < halfway && whereCenter + distanceToMove < halfway)
            mYOffset += distanceToMove;
    }
    else
    {
        if(whereCenter < halfway && whereCenter + distanceToMove < halfway)
            mYOffset += distanceToMove;
        else if(whereCenter > halfway && whereCenter - distanceToMove < halfway)
            mYOffset = (int)mYOffset + halfway;
        else if(whereCenter > halfway && whereCenter - distanceToMove > halfway)
            mYOffset -= distanceToMove;
    }
}

void Character::UpdateModel(const Direction dir)
{
    mModel = glm::translate(glm::mat4(1.0f),
                            glm::vec3(mXOffset, mYOffset, 0.0f));
    if(dir == Direction::Left)
    {
        // pi is 180 deg in radians
        mModel = glm::rotate(mModel, 3.14159f,
                              glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if(dir == Direction::Up)
    {
        // 1.57079 is half of pi so 90 deg
        mModel = glm::rotate(mModel, 1.57079f,
                              glm::vec3(0.0f, 0.0f, 1.0f));
    }
    else if(dir == Direction::Down)
    {
        mModel = glm::rotate(mModel, -1.57079f,
                              glm::vec3(0.0f, 0.0f, 1.0f));
    }
}
