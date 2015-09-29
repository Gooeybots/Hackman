#ifndef WRITEDATA_HPP
#define WRITEDATA_HPP

#include <vector>
#include <array>

struct Scores;

bool FileExists(const char * filename);
bool WriteData(const std::vector<float> &mAIVec, const std::array<unsigned int, 840> &map, const char * filename);
bool WriteHighscore(const std::vector<Scores> &scoreVec);

#endif // WRITEDATA_HPP
