#ifndef SETUPOBJECTS_HPP
#define SETUPOBJECTS_HPP

#include <vector>
#include <memory>

class VisibleObject;
class Map;
class ResourceManager;
class AI;

void GetVecFullOfObjects(std::vector<std::shared_ptr<VisibleObject> > &vec,
                         const Map &map, ResourceManager &resMan,
                         std::vector<AI> &aiVec);

#endif // SETUPOBJECTS_HPP
