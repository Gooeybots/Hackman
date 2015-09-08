#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "visibleobject.hpp"

VisibleObject::VisibleObject(const float xOffset, const float yOffset,
                             const std::shared_ptr<unsigned int> vao, const std::shared_ptr<unsigned int> nextVao,
                             const std::shared_ptr<unsigned int> texture, const std::shared_ptr<unsigned int> program):
    mXOffset(xOffset), mYOffset(yOffset), mVao(vao), mNextVao(nextVao), mTexture(texture),
    mProgram(program){}

VisibleObject::~VisibleObject(){}

void VisibleObject::SwitchVaos()
{
    std::shared_ptr<unsigned int> temp(mVao);
    mVao = mNextVao;
    mNextVao = temp;
}

void VisibleObject::TakeLife(){}

void VisibleObject::ResetToOriginalSquare(){}

glm::mat4 VisibleObject::GetModel()
{
    return glm::mat4(1.0f);
}

bool VisibleObject::Move(const Direction dir, const float dt, const Map &map)
{
    return false;
}

void VisibleObject::Draw(const glm::mat4 &view, const glm::vec4 &colour)
{
    if(mVao)
    {
        glUseProgram(*mProgram);
        glBindVertexArray(*mVao);

        glUniform2f(glGetUniformLocation(*mProgram, "offset"), mXOffset, mYOffset);
        glUniformMatrix4fv(glGetUniformLocation(*mProgram, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));
        glUniform4fv(glGetUniformLocation(*mProgram, "colour"), 1, glm::value_ptr(colour));

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);
        glUseProgram(0);
    }
}

void VisibleObject::Draw(const glm::mat4 &view)
{
    if(mVao)
    {
        glUseProgram(*mProgram);

        glBindVertexArray(*mVao);
        glBindTexture(GL_TEXTURE_2D, *mTexture);

        glUniform2f(glGetUniformLocation(*mProgram, "offset"), mXOffset, mYOffset);
        glUniformMatrix4fv(glGetUniformLocation(*mProgram, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));

        glDrawArrays(GL_TRIANGLE_FAN, 0 , 4);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
}

Direction VisibleObject::GetPrevDirection()
{
    return Direction::None;
}

std::shared_ptr<unsigned int> VisibleObject::GetTexture()
{
    return mTexture;
}

std::shared_ptr<unsigned int> VisibleObject::GetVao()
{
    return mVao;
}

std::shared_ptr<unsigned int> VisibleObject::GetProgram()
{
    return mProgram;
}

unsigned int VisibleObject::GetPlayer()
{
    return 0;
}

unsigned int VisibleObject::GetLives()
{
    return 0;
}

float VisibleObject::GetX()
{
    return mXOffset;
}

float VisibleObject::GetY()
{
    return mYOffset;
}

void VisibleObject::SetX(const float x)
{
    mXOffset = x;
}

void VisibleObject::SetY(const float y)
{
    mYOffset = y;
}
