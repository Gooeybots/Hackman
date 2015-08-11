#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include "setupprogram.hpp"

struct Shader
{
    const char * filename;
    GLenum shaderType;
};

GLuint CreateProgram(const std::vector<Shader> &shaderVec);
bool ReadToStream(const char * filename, std::stringstream &data);
void CreateShader(const char * data, GLuint shader);
void GetShaderError(const GLuint shader);
void GetProgramError(const GLuint program);

GLuint SetupProgram(const char * filenameVS, const char * filenameFS)
{
    Shader shaders;
    shaders.filename = filenameVS;
    shaders.shaderType = GL_VERTEX_SHADER;

    std::vector<Shader> shaderVec;
    shaderVec.push_back(shaders);

    shaders.filename = filenameFS;
    shaders.shaderType = GL_FRAGMENT_SHADER;

    shaderVec.push_back(shaders);

    return CreateProgram(shaderVec);
}

GLuint CreateProgram(const std::vector<Shader> &shaderVec)
{
    GLuint program(glCreateProgram());

    if(program == 0)
        std::cerr << "Couldn't create program\n";
    else
    {
        for(auto const &it : shaderVec)
        {
            GLuint shader = glCreateShader(it.shaderType);
            if(shader == 0)
            {
                glDeleteProgram(program);
                std::cerr << "Couldn't create shader\n";
            }
            else
            {
                std::stringstream data;
                ReadToStream(it.filename, data);
                CreateShader(data.str().c_str(), shader);

                GLint success = 0;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if(success == GL_FALSE)
                    GetShaderError(shader);
                else
                    glAttachShader(program, shader);

                glDeleteShader(shader);
                data.clear();
            }
        }
        glLinkProgram(program);

        GLint isLinked(0);
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if(isLinked == GL_FALSE)
        {
            GetProgramError(program);
            glDeleteProgram(program);
            program = 0;
        }
    }

    return program;
}

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

void CreateShader(const char * data, GLuint shader)
{
    glShaderSource(shader, 1, &data, NULL);
    glCompileShader(shader);
}

void GetShaderError(const GLuint shader)
{
    GLint length(0);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    char error[length + 1];
    glGetShaderInfoLog(shader, length + 1, NULL, error);
    for(int len(0); len < length; ++len)
        std::cerr << error << "\n";

    glDeleteShader(shader);
}

void GetProgramError(const GLuint program)
{
    GLint length(0);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    char error[length + 1];
    glGetProgramInfoLog(program, length + 1, NULL, error);
    for(int len(0); len < length; ++len)
        std::cerr << error << "\n";

    glDeleteProgram(program);
}
