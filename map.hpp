#ifndef MAP_HPP
#define MAP_HPP

#include <array>
#include <vector>
#include <glm/vec2.hpp>

class VisibleObject;

enum Object
{
    none,
    player,
    enemy1,
    enemy2,
    enemy3,
    enemy4,
    tree,
    choppedTree,
    block1,
    block2,
    wolfEntrance,
    powerPill,
    specialObject,
    passThrough
};

class Map
{
public:
    Map(const char * mapsFilename = "maps.txt");

// Returns false if last map was the last level
    bool LoadNextMap();
// sets object
    void SetObject(const glm::ivec2 &whichSquare, const unsigned int player);
    Object GetWhichObject(const glm::ivec2 &whichSquare) const;
private:
// Returns false if file couldn't be read
    bool RetriveMapFromFile();

    unsigned int mMap[28][30];
    std::vector<const char *> mMapFiles;
    std::vector<const char *>::iterator mWhichMap;
};

#endif // MAP_HPP
