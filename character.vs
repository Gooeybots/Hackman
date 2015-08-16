#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoordsIn;

uniform mat4 modelView;

out vec2 texCoords;

void main()
{
    texCoords = texCoordsIn;
    gl_Position = modelView * position;
}
