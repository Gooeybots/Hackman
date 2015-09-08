#include <fstream>
#include "writedata.hpp"

bool WriteData(const std::vector<float> &mAIVec, const std::array<unsigned int, 840> &map, const char *filename)
{
    bool retValue(false);
    unsigned int enemy(0);
    if(!FileExists(filename))
    {
        std::ofstream file(filename, std::ios::out);
        if(file.is_open())
        {
            for(auto & num : map)
            {
                file << num << " ";
                if(num == 10 || num == 11 || num == 12 || num == 13)
                {

                    std::vector<float>::const_iterator where(mAIVec.begin() + (enemy * 4));
                    file << *(where) << " " << *(where + 1) << " " << *(where + 2) << " " << *(where + 3) << " ";
                    enemy += 1;
                }
            }
            file.close();
            retValue = true;
        }
    }
    return retValue;
}

bool FileExists(const char *filename)
{
    bool retValue(false);
    std::ifstream file(filename, std::ios::in);
    if(file.is_open())
    {
        retValue = true;
        file.close();
    }
    return retValue;
}
