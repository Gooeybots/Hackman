#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

class ResourceManager;

void MakeMap(ResourceManager &resMan);

struct PatrolArea
{
    PatrolArea(const unsigned int playerIn, const float xIn,
               const float yIn, const float x1In, const float y1In)
        :player(playerIn), x(xIn), y(yIn), x1(x1In), y1(y1In){}
    unsigned int player;
    float x;
    float y;
    float x1;
    float y1;
};

#endif // MAPEDITOR_HPP
