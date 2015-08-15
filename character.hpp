#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <glm/vec2.hpp>
#include "visibleobject.hpp"

class Map;

class Character : public VisibleObject
{
public:
    Character(const float x, const float y, const float speed, const unsigned int player,
              const unsigned int vao, const unsigned int nextVao,
              const unsigned int texture, const unsigned int program,
              Direction = Direction::Right);
    ~Character();

    void Draw(const glm::mat4 &View);
/* moves character speed * dt and returns true if character moved square */
    bool Move(const Direction dir, const float dt, const Map &map);
    unsigned int GetPlayer(); // will return 0 if not a player

protected:
    bool CanMoveWithoutChangingSquare(const glm::vec2 &where,
                                      const float distanceToMove, const Direction dir);
    void MoveToCenter(const Direction dir);


    Direction prevDir;
    float currTime, prevTime;
    float mSpeed;
    unsigned int mPlayer;
};

#endif // CHARACTER_HPP
