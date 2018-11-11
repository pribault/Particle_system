#version 410 core

layout(location = 0) in vec4	v;
layout(location = 1) in vec4	c;

out vec4    cin;

void main()
{
    gl_Position.xy = v.xy;
    cin = c;
}
