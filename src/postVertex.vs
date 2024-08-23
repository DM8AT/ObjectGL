#version 450 core

layout (location = 0) in uint v_posIdx;

const vec2[] pos = vec2[](
    vec2(-1, 1),
    vec2(-1,-1),
    vec2( 1, 1),
    vec2( 1,-1)
);

out vec2 f_tex;

void main()
{
    gl_Position = vec4(pos[v_posIdx],0,1);
    f_tex = gl_Position.xy * 0.5 + 0.5;
}