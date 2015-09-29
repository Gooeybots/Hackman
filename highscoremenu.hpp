#ifndef HIGHSCOREMENU_HPP
#define HIGHSCOREMENU_HPP

class ResourceManager;

struct Scores
{
    unsigned int score;
    std::string name;
};

void UpdateHighscore(const unsigned int score, ResourceManager &resourceManager);
void DisplayHighscores(ResourceManager &resourceManager);

#endif // HIGHSCOREMENU_HPP
