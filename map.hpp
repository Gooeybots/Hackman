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
    Map();
    Map(const char * filename);

// Returns false if last map was the last level
    bool HasFinished();
    bool LoadNextMap();
    bool CanMove(const Direction dir, const glm::ivec2 &where, const bool active = false) const;
    bool UpdateSpecialObj(const glm::ivec2 &whichSquare);
    void HideSpecialObj();
// sets object
    void SetObject(const glm::ivec2 &whichSquare);
    Object GetWhichObject(const glm::ivec2 &whichSquare) const;
    glm::ivec2 FindObject(const Object obj);
    std::array<unsigned int, 840> GetMap();

    std::vector<float> mAiPatrolPositions;
private:
// Returns false if file couldn't be read
    bool RetriveMapFromFile();
    bool CanTravelDirection(const unsigned int x, const unsigned int y,
                            const Direction dir) const;
    bool CanPassThroughObject(const Object obj, const bool active) const;

    unsigned int mTrees;
    std::array<unsigned int, 840> mMap;
    std::vector<std::string> mMapFiles;
    std::vector<std::string>::iterator mWhichMap;
};

#endif // MAP_HPP
