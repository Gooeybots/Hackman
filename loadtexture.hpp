#ifndef LOADTEXTURE_HPP
#define LOADTEXTURE_HPP

#include <vector>

/* Creates texture from the  .png supplied from filename returns
 * 0 if texture couldnt be created otherwise returns a texture GLuint */
void TextureToVec(const std::string &filename, std::vector<unsigned char> &imageVec, unsigned int &width, unsigned int &height);
unsigned int LoadTexture(const char * filename);
unsigned int CreateOpenGLTexture(std::vector<unsigned char> &imageVec, const unsigned int &width, const unsigned int &height);

#endif // LOADTEXTURE_HPP
