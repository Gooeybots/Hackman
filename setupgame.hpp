#ifndef SETUPGAME_HPP
#define SETUPGAME_HPP

class ResourceManager;

void CreateResources(ResourceManager &resourceManager);
bool SetupGame(ResourceManager &resourceManager, const unsigned int lives);

#endif // SETUPGAME_HPP
