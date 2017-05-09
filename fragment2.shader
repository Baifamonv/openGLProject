#version 330 core

out vec4 color;
// color will be set in OpenGL code
uniform vec4 ourColor;

void main()
{
    color = ourColor ;
} 