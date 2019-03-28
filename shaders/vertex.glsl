#version 410 core

layout(location = 0) in vec4	v;
layout(location = 1) in vec4	c;

uniform mat4	view;
uniform mat4	projection;
uniform mat4	model;

out vec4	cin;

void	main(void)
{
	gl_Position = (projection * view * model * vec4(v.xyz, 1));
	cin = c;
}
