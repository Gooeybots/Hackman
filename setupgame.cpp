#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include "setupgame.hpp"
#include "resourcemanager.hpp"
#include "readtostream.hpp"
#include "map.hpp"
#include "visibleobject.hpp"
#include "playgame.hpp"

GLuint CreateBuffer();
GLuint CreateVAO(const GLuint buffer, const unsigned int whichBuffer);
GLuint SetupStandardVAO(const GLuint buffer);

bool SetupGame()
{
    ResourceManager resourceManager;
    Map map;

    resourceManager.CreateTexture("geoff.png");
    resourceManager.CreateTexture("wolf.png");
    resourceManager.CreateTexture("bear.png");
    resourceManager.CreateTexture("scenery.png");
    resourceManager.CreateProgram("textured.vs", "textured.fs");
    resourceManager.CreateProgram("character.vs", "textured.fs");

    {
        GLuint buffer(CreateBuffer());
        GLuint vao(CreateVAO(buffer, 1));
        GLuint vaoTopLeft(CreateVAO(buffer, 2));
        GLuint vaoTopRight(CreateVAO(buffer, 3));
        GLuint vaoBottomLeft(CreateVAO(buffer, 4));
        GLuint vaoBottomRight(CreateVAO(buffer, 5));

        resourceManager.AddVao("buffer", buffer);
        resourceManager.AddVao("full vao", vao);
        resourceManager.AddVao("vao top left", vaoTopLeft);
        resourceManager.AddVao("vao top right", vaoTopRight);
        resourceManager.AddVao("vao bottom left", vaoBottomLeft);
        resourceManager.AddVao("vao bottom right", vaoBottomRight);
    }

    return PlayGame(map, resourceManager);
}

GLuint CreateBuffer()
{
    std::vector<float> positions;
// positions for square from a fan
    positions.push_back( 0.5f); positions.push_back(-0.5f);
    positions.push_back( 0.5f); positions.push_back( 0.5f);
    positions.push_back(-0.5f); positions.push_back( 0.5f);
    positions.push_back(-0.5f); positions.push_back(-0.5f);
// tex coords full
    positions.push_back(1.0f); positions.push_back(1.0f);
    positions.push_back(1.0f); positions.push_back(0.0f);
    positions.push_back(0.0f); positions.push_back(0.0f);
    positions.push_back(0.0f); positions.push_back(1.0f);
// tex coords top left
    positions.push_back(0.5f); positions.push_back(0.5f);
    positions.push_back(0.5f); positions.push_back(0.0f);
    positions.push_back(0.0f); positions.push_back(0.0f);
    positions.push_back(0.0f); positions.push_back(0.5f);
// tex coords top right
    positions.push_back(1.0f); positions.push_back(0.5f);
    positions.push_back(1.0f); positions.push_back(0.0f);
    positions.push_back(0.5f); positions.push_back(0.0f);
    positions.push_back(0.5f); positions.push_back(0.5f);
// tex coords bottom left
    positions.push_back(0.5f); positions.push_back(1.0f);
    positions.push_back(0.5f); positions.push_back(0.5f);
    positions.push_back(0.0f); positions.push_back(0.5f);
    positions.push_back(0.0f); positions.push_back(1.0f);
// tex coords bottom right
    positions.push_back(1.0f); positions.push_back(1.0f);
    positions.push_back(1.0f); positions.push_back(0.5f);
    positions.push_back(0.5f); positions.push_back(0.5f);
    positions.push_back(0.5f); positions.push_back(1.0f);

    GLuint buffer(0);

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(),
                 positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    positions.clear();

    return buffer;
}

GLuint CreateVAO(const GLuint buffer, const unsigned int whichBuffer)
{
    GLuint vao(SetupStandardVAO(buffer));

    glEnableVertexAttribArray(1);
    std::size_t offset(sizeof(float) * 2 * 4 * whichBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offset);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
}

GLuint SetupStandardVAO(const GLuint buffer)
{
    GLuint vao(0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    return vao;
}
