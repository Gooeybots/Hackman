#include <GLFW/glfw3.h>
#include "enemy.hpp"

Enemy::Enemy(const float x, const float y, const float speed, const unsigned int player,
             const std::shared_ptr<unsigned int> &vao, const std::shared_ptr<unsigned int> &nextVao,
             const std::shared_ptr<unsigned int> &texture, const std::shared_ptr<unsigned int> &program,
             const std::shared_ptr<unsigned int> &deathVao, const std::shared_ptr<unsigned int> &frightenedVao,
             const Direction dir):
    Character(x, y, speed, player, vao, nextVao, texture, program, deathVao, dir),
    mCanDie(false), mFrightenedTex(false), mFrightenedVao(frightenedVao){}

bool Enemy::GetCanDie()
{
    return mCanDie;
}

bool Enemy::Move(const Direction dir, const float dt, const Map &map)
{
    bool moved(false);

    if(mActive)
    {
        UpdateAnimation();
        moved = GetMovement(dir, dt, map);
    }
    return moved;
}

void Enemy::ChangeCanDie()
{
    mCanDie = !mCanDie;
    if(!mCanDie)
    {
       if(mFrightenedTex)
       {
           mFrightenedTex = !mFrightenedTex;
           std::shared_ptr<unsigned int> next(mVao);
           mVao = mFrightenedVao;
           mFrightenedVao = next;
       }
    }
    currTime = prevTime = glfwGetTime();
}

void Enemy::UpdateAnimation()
{
    currTime = glfwGetTime();
    if(currTime - prevTime >= 0.2) // should probably change from magic no
    {
        if(mCanDie)
        {
            mFrightenedTex = !mFrightenedTex;
            std::shared_ptr<unsigned int> next(mVao);
            mVao = mFrightenedVao;
            mFrightenedVao = next;
        }
        else
        {
            SwitchVaos();
        }
        prevTime = currTime;
    }
}

void Enemy::SwitchDeathVao()
{
    if(mFrightenedTex)
    {
        std::shared_ptr<unsigned int> next(mVao);
        mVao = mFrightenedVao;
        mFrightenedVao = next;
        mFrightenedTex = !mFrightenedTex;
    }
    Character::SwitchDeathVao();
}
