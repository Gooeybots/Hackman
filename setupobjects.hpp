#ifndef SETUPOBJECTS_HPP
#define SETUPOBJECTS_HPP

#include <vector>
#include <memory>

class VisibleObject;
class Map;
class ResourceManager;
class AI;
class PatrolArea;

bool GetVecFullOfObjects(std::vector<std::shared_ptr<VisibleObject> > &vec, Map &map,
                         ResourceManager &resMan, std::vector<PatrolArea> &patVec);

bool GetVecFullOfObjects(std::vector<std::shared_ptr<VisibleObject> > &vec, Map &map,
                         ResourceManager &resMan, std::vector<std::shared_ptr<AI>> &aiVec);

std::shared_ptr<VisibleObject> GetPlayer(const unsigned int player,
                                         std::vector<std::shared_ptr<VisibleObject>> &objVec);

#endif // SETUPOBJECTS_HPP
