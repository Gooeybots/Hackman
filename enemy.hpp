#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "character.hpp"

class Enemy : public Character
{
public:
    Enemy(const float x, const float y, const float speed, const unsigned int player,
          const std::shared_ptr<unsigned int> &vao, const std::shared_ptr<unsigned int> &nextVao,
          const std::shared_ptr<unsigned int> &texture, const std::shared_ptr<unsigned int> &program,
          const std::shared_ptr<unsigned int> &deathVao, const std::shared_ptr<unsigned int> &frightenedVao,
          const Direction dir = Direction::Right);

    bool Move(const Direction dir, const float dt, const Map &map);
    bool GetCanDie();
    void ChangeCanDie();
    void SwitchDeathVao();

protected:
    void UpdateAnimation();

    bool mCanDie;
    bool mFrightenedTex;
    std::shared_ptr<unsigned int> mFrightenedVao;
};

#endif // ENEMY_HPP
