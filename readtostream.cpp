#include <iostream>
#include <fstream>
#include <sstream>
#include "readtostream.hpp"

bool ReadToStream(const char * filename, std::stringstream &data)
{
    std::ifstream file(filename, std::ios::in);

    if(file.is_open())
    {
        data << file.rdbuf();
        file.close();
    }
    else
    {
        std::cerr << "Couldn't open file " << filename << "\n";
        return false;
    }
    return true;
}
