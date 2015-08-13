#ifndef SETUPOBJECTS_HPP
#define SETUPOBJECTS_HPP

#include <vector>
#include <memory>

class VisibleObject;
class Map;
class ResourceManager;

void GetVecFullOfObjects(std::vector<std::shared_ptr<VisibleObject> > &vec,
                         const Map &map, ResourceManager &resMan);

#endif // SETUPOBJECTS_HPP
