#version 460 core

out vec4 p_col;

in vec3 f_norm;
in vec2 f_tex;
in vec3 f_locPos;

uniform sampler2D tex;

void main()
{
    vec3 col;
    col = texture(tex, f_tex).rgb;
    p_col = vec4(col,1);
}