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
        GLuint object(*(id.second));
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

std::shared_ptr<unsigned int> ResourceManager::GetVao(const std::string &name)
{
    return GetData(name);
}

std::shared_ptr<unsigned int> ResourceManager::GetProgram(const std::string &vs, const std::string &fs)
{
    std::string name(vs + " " + fs);
    std::shared_ptr<unsigned int> program(GetData(name));
    if(program == nullptr)
    {
        program = std::make_shared<unsigned int>(
                    SetupProgram(vs.c_str(), fs.c_str()));
        AddToMap(name, program);
    }
    return program;
}

std::shared_ptr<unsigned int> ResourceManager::GetTexture(const std::string &filename)
{
    std::shared_ptr<unsigned int> texture(GetData(filename));
    if(texture == nullptr)
    {
        texture = std::make_shared<unsigned int>(
                    LoadTexture(filename.c_str()));
        AddToMap(filename, texture);
    }
    return texture;
}

std::shared_ptr<unsigned int> ResourceManager::CreateProgram(const std::string &vs, const std::string &fs)
{
    std::shared_ptr<unsigned int> program(
                new unsigned int(SetupProgram(vs.c_str(), fs.c_str())));
    std::string name(vs + " " + fs);
    AddToMap(name, program);
    return program;
}

std::shared_ptr<unsigned int> ResourceManager::CreateTexture(const std::string &filename)
{
    std::shared_ptr<unsigned int> texture(
                new unsigned int (LoadTexture(filename.c_str())));
    AddToMap(filename, texture);
    return texture;
}

void ResourceManager::AddVao(const std::string &name, const std::shared_ptr<unsigned int> vao)
{
    AddToMap(name, vao);
}

std::shared_ptr<unsigned int> ResourceManager::GetData(const std::string &name)
{
    auto object(mMap.find(name));
    if(object != mMap.end())
        return object->second;
    return nullptr;
}

void ResourceManager::AddToMap(const std::string &name, const std::shared_ptr<unsigned int> &id)
{
    mMap.insert(std::pair<std::string, std::shared_ptr<unsigned int>>(name, id));
}
