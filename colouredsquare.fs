#version 330 core

smooth out vec4 finalColour;

uniform vec4 colour;

void main()
{
    finalColour = colour;
}
