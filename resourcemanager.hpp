#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <map>
#include <string>

class ResourceManager
{
public:
    ResourceManager();
// This will delete all the vao's, textures
    ~ResourceManager();
/* This will return the unsigned int if it can find the vao if not it will
 * return 0 */
    unsigned int GetVao(const std::string &name);
/* These will return the unsigned int of the given filenames if it doesn't exist
 * it will create the object then return the unsigned int */
    unsigned int GetTexture(const std::string &filename);
    unsigned int GetProgram(const std::string &vs, const std::string &fs);
/* These will create the object same as above without the search for it. It will
 * return 0 if there was a problem */
    unsigned int CreateTexture(const std::string &filename);
    unsigned int CreateProgram(const std::string &vs, const std::string &fs);
// Will add the vao under the name to the map
    void AddVao(const std::string &name, const unsigned int vao);
private:
    void AddToMap(const std::string &name, const unsigned int &id);
    unsigned int GetData(const std::string &name);
    std::map<std::string, unsigned int> mMap;
};

#endif // RESOURCEMANAGER_HPP
