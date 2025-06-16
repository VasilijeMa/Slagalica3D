#version 330 core

in vec2 chTex;
out vec4 outCol;

uniform sampler2D foreground;

void main()
{   
    vec4 tex = texture(foreground, chTex);
    if (tex.a == 0.0) discard;
    outCol = vec4(tex.rgba);
}