#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "lodepng.h"
#include "loadtexture.hpp"

void FlipTextureY(std::vector<unsigned char> &image, const unsigned int width, const unsigned int height);

void TextureToVec(const std::string &filename, std::vector<unsigned char> &imageVec, unsigned int &width, unsigned int &height)
{
    unsigned int error(lodepng::decode(imageVec, width, height, filename));
    if(error)
    {
        std::cerr << "Couldn't load " << filename << "\n";
    }
    else
    {
        FlipTextureY(imageVec, width, height); // To put image the right way up for opengl
    }
}

unsigned int CreateOpenGLTexture(std::vector<unsigned char> &imageVec, const unsigned int &width, const unsigned int &height)
{
    GLuint texture(0);
    glGenTextures(1, &texture);
    if(texture == 0)
    {
        std::cerr << "Couldn't generate texture\n";
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, imageVec.data());

        imageVec.clear();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        GLenum error(glGetError());
        if(error != GL_NO_ERROR)
        {
            std::cerr << error << " Problem allocating texture\n";
            glDeleteTextures(1, &texture);
            texture = 0;
        }
    }
    return texture;
}

unsigned int LoadTexture(const char * filename)
{
    std::vector<unsigned char> image;
    GLuint width, height, texture(0);
    unsigned int error(lodepng::decode(image, width, height, filename));
    if(error)
    {
        std::cerr << "Couldn't load " << filename << "\n";
    }
    else
    {
        FlipTextureY(image, width, height); // To put image the right way up for opengl

        glGenTextures(1, &texture);
        if(texture == 0)
        {
            std::cerr << "Couldn't generate texture " << filename << "\n";
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
                std::cerr << error << " Problem allocating texture " << filename << "\n";
                glDeleteTextures(1, &texture);
                texture = 0;
            }
        }
    }
    return texture;
}

void FlipTextureY(std::vector<unsigned char> &image, const unsigned int width,
                  const unsigned int height)
{
    unsigned int size(width * 4);
    std::vector<unsigned char> newImage = image;
    auto vec(newImage.begin() += newImage.size() - size);
    image.clear();
    for(unsigned int y(0); y < height; ++y)
    {
        for(unsigned int x(0); x < size; ++x)
        {
            image.push_back(*vec);
            ++vec;
        }
        vec -= (size * 2);
    }
}
