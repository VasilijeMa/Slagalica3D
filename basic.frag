#version 330 core

in vec3 channelCol;
out vec4 outCol;
uniform bool white;

void main()
{
	if (white) {
		outCol = vec4(1.0, 1.0, 1.0, 1.0);
	} else {
		outCol = vec4(channelCol, 1.0);
	}
}