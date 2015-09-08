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
#include "createbuffer.hpp"

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
        std::shared_ptr<unsigned int> buffer(new unsigned int (CreateBuffer()));
        std::shared_ptr<unsigned int> vao(new unsigned int (CreateVAO(*buffer, 1)));
        std::shared_ptr<unsigned int> vaoTopLeft(new unsigned int (CreateVAO(*buffer, 2)));
        std::shared_ptr<unsigned int> vaoTopRight(new unsigned int (CreateVAO(*buffer, 3)));
        std::shared_ptr<unsigned int> vaoBottomLeft(new unsigned int (CreateVAO(*buffer, 4)));
        std::shared_ptr<unsigned int> vaoBottomRight(new unsigned int (CreateVAO(*buffer, 5)));

        resourceManager.AddVao("buffer", buffer);
        resourceManager.AddVao("full vao", vao);
        resourceManager.AddVao("vao top left", vaoTopLeft);
        resourceManager.AddVao("vao top right", vaoTopRight);
        resourceManager.AddVao("vao bottom left", vaoBottomLeft);
        resourceManager.AddVao("vao bottom right", vaoBottomRight);
    }

    return PlayGame(map, resourceManager);
}
