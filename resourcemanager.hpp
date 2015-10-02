#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <map>
#include <string>
#include <memory>
#include <AL/alc.h>

class ResourceManager
{
public:
    ResourceManager();
// This will delete all the vao's, textures
    ~ResourceManager();

/* This will return the unsigned int if it can find the vao if not it will
 * return nullptr */
    std::shared_ptr<unsigned int> GetVao(const std::string &name);
    std::shared_ptr<unsigned int> GetTexture(const std::string &filename);
    std::shared_ptr<unsigned int> GetProgram(const std::string &vs, const std::string &fs);
    std::shared_ptr<unsigned int> GetSound(const std::string &filename);
/* These will return the unsigned int of the given filenames if it doesn't exist
 * it will create the object then return the unsigned int */
    std::shared_ptr<unsigned int> GetOrCreateTexture(const std::string &filename);
    std::shared_ptr<unsigned int> GetOrCreateProgram(const std::string &vs, const std::string &fs);
    std::shared_ptr<unsigned int> GetOrCreateSound(const std::string &filename, ALCcontext * context);
/* These will create the object same as above without the search for it. It will
 * return nullptr if there was a problem */
    std::shared_ptr<unsigned int> CreateTexture(const std::string &filename);
    std::shared_ptr<unsigned int> CreateProgram(const std::string &vs, const std::string &fs);
    std::shared_ptr<unsigned int> CreateSound(const std::string &filename, ALCcontext * context);
// Will add the vao under the name to the map
    void AddTexture(const std::string &filename, const std::shared_ptr<unsigned int> &texture);
    void AddProgram(const std::string &vs, const std::string &fs, const std::shared_ptr<unsigned int> &program);
    void AddVao(const std::string &name, const std::shared_ptr<unsigned int> &vao);
    void AddSound(const std::string &name, const std::shared_ptr<unsigned int> &sound);
private:
    void AddToMap(const std::string &name, const std::shared_ptr<unsigned int> &id);
    std::shared_ptr<unsigned int> GetData(const std::string &name);

    std::map<std::string, std::shared_ptr<unsigned int>> mMap;
};

#endif // RESOURCEMANAGER_HPP
