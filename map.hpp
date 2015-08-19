#ifndef MAP_HPP
#define MAP_HPP

#include <array>
#include <vector>
#include <glm/vec2.hpp>
#include "visibleobject.hpp"

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
    bool HasFinished();
    bool LoadNextMap();
    bool CanMove(const Direction dir, const glm::ivec2 &where) const;
// sets object
    void SetObject(const glm::ivec2 &whichSquare, const unsigned int player);
    Object GetWhichObject(const glm::ivec2 &whichSquare) const;
    glm::ivec2 FindObject(const Object obj);
    std::array<unsigned int, 840> GetMap();
private:
// Returns false if file couldn't be read
    bool RetriveMapFromFile();
    bool CanTravelDirection(const unsigned int x, const unsigned int y,
                            const Direction dir) const;
    bool CanPassThroughObject(const Object obj) const;

    unsigned int mTrees;
    std::array<unsigned int, 840> mMap;
    std::vector<const char *> mMapFiles;
    std::vector<const char *>::iterator mWhichMap;
};

#endif // MAP_HPP
