#version 330 core

in vec2 texCoords;

smooth out vec4 finalColour;

uniform vec4 colour;
uniform sampler2D tex;

void main()
{
    finalColour = texture(tex, texCoords) * colour;
}
