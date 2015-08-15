#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "visibleobject.hpp"

VisibleObject::VisibleObject(const float xOffset, const float yOffset,
                             const unsigned int vao, const unsigned int nextVao,
                             const unsigned int texture, const unsigned int program):
    mXOffset(xOffset), mYOffset(yOffset), mVao(vao), mNextVao(nextVao), mTexture(texture),
    mProgram(program){}

VisibleObject::~VisibleObject(){}

void VisibleObject::SwitchVaos()
{
    GLuint temp(mVao);
    mVao = mNextVao;
    mNextVao = temp;
}

bool VisibleObject::Move(const Direction dir, const float dt, const Map &map)
{
    return false;
}

void VisibleObject::Draw(const glm::mat4 &view)
{
    glUseProgram(mProgram);

    glBindVertexArray(mVao);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glUniform2f(glGetUniformLocation(mProgram, "offset"), mXOffset, mYOffset);
    glUniformMatrix4fv(glGetUniformLocation(mProgram, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));

    glDrawArrays(GL_TRIANGLE_FAN, 0 , 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

unsigned int VisibleObject::GetPlayer()
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
