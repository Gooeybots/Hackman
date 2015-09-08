#version 330 core

layout(location = 0) in vec4 position;

uniform vec2 offset;
uniform mat4 view;

void main()
{
    gl_Position = view * (position + vec4(offset, 0.0, 0.0));
}
