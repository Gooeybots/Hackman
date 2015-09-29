#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <glm/vec2.hpp>
#include "visibleobject.hpp"

class Map;

class Character : public VisibleObject
{
public:
    Character(const float x, const float y, const float speed, const unsigned int player,
              const std::shared_ptr<unsigned int> &vao, const std::shared_ptr<unsigned int> &nextVao,
              const std::shared_ptr<unsigned int> &texture, const std::shared_ptr<unsigned int> &program,
              const std::shared_ptr<unsigned int> &deathVao, const Direction dir = Direction::Right);
    virtual ~Character();

    void Draw(const glm::mat4 &view);
    void TakeLife();
    void ResetToOriginalSquare();
    void SwitchDeathVao();
/* moves character speed * dt and returns true if character moved square */
    bool Move(const Direction dir, const float dt, const Map &map);
    bool GetActive();
    Direction GetPrevDirection();
    unsigned int GetPlayer(); // will return 0 if not a player
    glm::mat4 GetModel();

protected:
    bool GetMovement(const Direction dir, const float dt, const Map &map);
    bool CanMoveWithoutChangingSquare(const glm::vec2 &where,
                                      const float distanceToMove, const Direction dir);
    void MoveToCenter(const Direction dir, const float distanceToMove);
    void MoveToCenterVertical(const Direction dir, const float distanceToMove);
    void MoveToCenterHorizontal(const Direction dir, const float distanceToMove);
    virtual void UpdateAnimation();
    void UpdateModel(const Direction dir);

    Direction lastFullMove;
    Direction prevDir;
    unsigned int mPlayer;
    float currTime;
    float prevTime;
    float mSpeed;
    glm::vec2 mHomeSquare;
    glm::mat4 mModel;
    std::shared_ptr<unsigned int> mDeathVao;
    bool mActive;
};

#endif // CHARACTER_HPP
