#version 450 core

uniform sampler2D mainBuff;

layout (location = 0) out vec4 color;

in vec2 f_tex;

void main()
{
    color = vec4(vec3(1) - texture(mainBuff, f_tex).rgb,1);
}