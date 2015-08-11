#include <GL/glew.h>
#include "resourcemanager.hpp"
#include "loadtexture.hpp"
#include "setupprogram.hpp"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for(auto const &id : mMap)
    {
        GLuint object(id->second);
        if(glIsVertexArray(object))
            glDeleteVertexArrays(1, &object);
        else if(glIsTexture(object))
            glDeleteTextures(1, &object);
        else if(glIsProgram(object))
            glDeleteProgram(object);
        else if(glIsBuffer(object))
            glDeleteBuffers(1, &object);
    }
    mMap.clear();
}

unsigned int ResourceManager::GetVao(const std::string &name)
{
    return GetData(name);
}

unsigned int ResourceManager::GetProgram(const std::string &vs, const std::string &fs)
{
    std::string name(vs + " " + fs);
    unsigned int program(GetData(name));
    if(program == 0)
    {
        program = SetupProgram(vs.c_str(), fs.c_str());
        AddToMap(name, program);
    }
    return program;
}

unsigned int ResourceManager::GetTexture(const std::string &filename)
{
    unsigned int texture(GetData(name));
    if(texture == 0)
    {
        texture = LoadTexture(filename.c_str());
        AddToMap(name, texture);
    }
    return texture;
}

unsigned int ResourceManager::CreateProgram(const std::string &vs, const std::string &fs)
{
    unsigned int program(SetupProgram(vs.c_str(), fs.c_str()));
    std::string name(vs + " " + fs);
    AddToMap(name, program);
    return program;
}

unsigned int ResourceManager::CreateTexture(const std::string &filename)
{
    unsigned int texture(LoadTexture(filename.c_str()));
    AddToMap(filename, texture);
    return texture;
}

void ResourceManager::AddVao(const std::string &name, const unsigned int vao)
{
    AddToMap(name, vao);
}

unsigned int ResourceManager::GetData(const std::string &name)
{
    auto object(mMap.find(name));
    if(object != mMap.end())
        return object->second;
    return 0;
}

void ResourceManager::AddToMap(const std::string &name, const unsigned int &id)
{
    mMap.insert(std::pair<std::string, unsigned int>(name, id));
}
