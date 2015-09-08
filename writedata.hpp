#ifndef WRITEDATA_HPP
#define WRITEDATA_HPP

#include <vector>
#include <array>

bool FileExists(const char * filename);
bool WriteData(const std::vector<float> &mAIVec, const std::array<unsigned int, 840> &map, const char * filename);

#endif // WRITEDATA_HPP
