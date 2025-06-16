#version 330 core 

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inTex;
out vec2 chTex;

void main()
{
	gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
	chTex = vec2(inTex.s, inTex.t);
}