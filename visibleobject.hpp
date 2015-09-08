#ifndef VISIBLEOBJECT_HPP
#define VISIBLEOBJECT_HPP

#include <memory>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "directionenum.hpp"

class Map;

class VisibleObject
{
public:
    VisibleObject(const float xOffset, const float yOffset, const std::shared_ptr<unsigned int> vao,
                  const std::shared_ptr<unsigned int> nextVao, const std::shared_ptr<unsigned int> texture,
                  const std::shared_ptr<unsigned int> program);
    virtual ~VisibleObject();

    virtual void Draw(const glm::mat4 &view, const glm::vec4 &colour);
    virtual void Draw(const glm::mat4 &view);
    virtual void TakeLife();
    virtual void ResetToOriginalSquare();
    virtual bool Move(const Direction dir, const float dt, const Map &map);
    virtual Direction GetPrevDirection();
    virtual unsigned int GetPlayer(); // will return 0 if not a player
    virtual unsigned int GetLives();  // will return 0 if not a player
    virtual glm::mat4 GetModel();

    void SwitchVaos();
    std::shared_ptr<unsigned int> GetTexture();
    std::shared_ptr<unsigned int> GetVao();
    std::shared_ptr<unsigned int> GetProgram();
    float GetX();
    float GetY();
    void SetX(const float x);
    void SetY(const float y);

protected:
    float mXOffset, mYOffset;
    std::shared_ptr<unsigned int> mVao, mNextVao;
    std::shared_ptr<unsigned int> mTexture;
    std::shared_ptr<unsigned int> mProgram;
};

#endif // VISIBLEOBJECT_HPP
