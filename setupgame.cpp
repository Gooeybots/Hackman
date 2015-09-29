#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include "loadtexture.hpp"
#include "setupgame.hpp"
#include "resourcemanager.hpp"
#include "readtostream.hpp"
#include "map.hpp"
#include "visibleobject.hpp"
#include "playgame.hpp"
#include "createbuffer.hpp"

struct Textures
{
    Textures(const std::string file):filename(file){}
    std::string filename;
    std::vector<unsigned char> texVec;
    unsigned int width;
    unsigned int height;
};

void CreateTexture(std::vector<Textures> &textureVec);

bool SetupGame(ResourceManager &resourceManager, const unsigned int lives)
{
    Map map;
    CreateResources(resourceManager);

    return PlayGame(map, resourceManager, lives);
}

void CreateResources(ResourceManager &resourceManager)
{
    std::vector<Textures> texVec;

    if(!resourceManager.GetTexture("geoff.png"))
    {
        texVec.push_back(Textures("geoff.png"));
    }
    if(!resourceManager.GetTexture("wolf.png"))
    {
        texVec.push_back(Textures("wolf.png"));
    }
    if(!resourceManager.GetTexture("bear.png"))
    {
        texVec.push_back(Textures("bear.png"));
    }
    if(!resourceManager.GetTexture("scenery.png"))
    {
        texVec.push_back(Textures("scenery.png"));
    }
    if(!resourceManager.GetTexture("scenery2.png"))
    {
        texVec.push_back(Textures("scenery2.png"));
    }
    if(!resourceManager.GetTexture("text.png"))
    {
        texVec.push_back(Textures("text.png"));
    }
    if(!resourceManager.GetTexture("snake.png"))
    {
        texVec.push_back(Textures("snake.png"));
    }

    resourceManager.GetOrCreateProgram("textured.vs", "textured.fs");
    resourceManager.GetOrCreateProgram("character.vs", "textured.fs");
    resourceManager.GetOrCreateProgram("colouredsquare.vs", "colouredsquare.fs");

    std::shared_ptr<unsigned int> buffer(resourceManager.GetVao("buffer"));

    if(!buffer)
    {
        buffer = std::make_shared<unsigned int>(CreateBuffer());
        resourceManager.AddVao("buffer", buffer);
    }
    if(!resourceManager.GetVao("full vao"))
    {
        std::shared_ptr<unsigned int> vao(new unsigned int (CreateVAO(*buffer, 1)));
        resourceManager.AddVao("full vao", vao);
    }
    if(!resourceManager.GetVao("vao top left"))
    {
        std::shared_ptr<unsigned int> vaoTopLeft(new unsigned int (CreateVAO(*buffer, 2)));
        resourceManager.AddVao("vao top left", vaoTopLeft);
    }
    if(!resourceManager.GetVao("vao top right"))
    {
        std::shared_ptr<unsigned int> vaoTopRight(new unsigned int (CreateVAO(*buffer, 3)));
        resourceManager.AddVao("vao top right", vaoTopRight);
    }
    if(!resourceManager.GetVao("vao bottom left"))
    {
        std::shared_ptr<unsigned int> vaoBottomLeft(new unsigned int (CreateVAO(*buffer, 4)));
        resourceManager.AddVao("vao bottom left", vaoBottomLeft);
    }
    if(!resourceManager.GetVao("vao bottom right"))
    {
        std::shared_ptr<unsigned int> vaoBottomRight(new unsigned int (CreateVAO(*buffer, 5)));
        resourceManager.AddVao("vao bottom right", vaoBottomRight);
    }
    if(!resourceManager.GetVao("vao plain colour"))
    {
        std::shared_ptr<unsigned int> vaoPlainColour(new unsigned int (SetupStandardVAO(*buffer)));
        resourceManager.AddVao("vao plain colour", vaoPlainColour);
    }

    CreateTexture(texVec);
    for(auto object(texVec.begin()); object != texVec.end(); ++object)
    {
        std::shared_ptr<unsigned int> ptr(
                    new unsigned int(CreateOpenGLTexture((*object).texVec, (*object).width,
                                                         (*object).height)));
        resourceManager.AddTexture((*object).filename, ptr);
    }
    texVec.clear();
}

void CreateTexture(std::vector<Textures> &textureVec)
{
    std::vector<std::thread> threadVec;
    for(auto & object : textureVec)
    {
        threadVec.push_back(std::thread(TextureToVec, std::ref(object.filename),
                                        std::ref(object.texVec), std::ref(object.width),
                                        std::ref(object.height)));
    }
    for(auto & thread : threadVec)
    {
        thread.join();
    }
}
