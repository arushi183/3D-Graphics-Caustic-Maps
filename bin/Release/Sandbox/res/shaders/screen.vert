#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 frag_uv;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    frag_uv = aTex;
}