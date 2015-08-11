#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoordsIn;

uniform vec2 offset;
uniform vec3 colourIn;
uniform mat4 view;

out vec2 texCoords;
out vec3 colour;

void main()
{
    colour = colourIn;
    texCoords = texCoordsIn;
    gl_Position = view * (position + vec4(offset, 0.0, 0.0));
}
