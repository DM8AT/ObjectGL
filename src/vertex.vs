#version 460 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_norm;
layout (location = 2) in vec2 v_tex;

out vec3 f_norm;
out vec2 f_tex;
out vec3 f_locPos;

layout (binding = 0, std140) uniform data
{
    mat4 transf;
    mat4 rot;
    float aspect;
};

layout (binding = 1, std140) uniform proj
{
    mat4 pMat;
};

void main()
{
    f_norm = (vec4(v_norm.zyx,1) * rot).zyx;
    f_tex = v_tex;
    f_locPos = v_pos;
    gl_Position = (vec4(v_pos, 1)*rot)*transf;
    gl_Position *= pMat;
    gl_Position.x *= aspect;
}