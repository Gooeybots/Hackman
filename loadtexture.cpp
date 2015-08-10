#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "lodepng.h"
#include "loadtexture.hpp"

GLuint LoadTexture(const char * filename)
{
    std::vector<unsigned char> image;
    GLuint width, height, texture(0);
    unsigned int error(lodepng::decode(image, width, height, filename));

    if(error)
    {
        std::cout << "Couldn't load " << filename << "\n";
    }
    else
    {
        glGenTextures(1, &texture);
        if(texture == 0)
        {
            std::cout << "Couldn't generate texture " << filename << "\n";
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, image.data());

            image.clear();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, 0);
            GLenum error(glGetError());
            if(error != GL_NO_ERROR)
            {
                std::cout << error << " Problem allocating texture " << filename << "\n";
                glDeleteTextures(1, &texture);
                texture = 0;
            }
        }
    }
    return texture;
}
