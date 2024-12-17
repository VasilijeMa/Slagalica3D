#version 330 core

in vec3 channelCol;
out vec4 outCol;

void main()
{
	outCol = vec4(channelCol, 1.0);
}