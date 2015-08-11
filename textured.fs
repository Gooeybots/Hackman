#version 330 core

in vec2 texCoords;
in vec3 colour;

smooth out vec4 color;

uniform sampler2D tex;

void main()
{
    color = texture(tex, texCoords) * vec4(colour, 1.0);
}
