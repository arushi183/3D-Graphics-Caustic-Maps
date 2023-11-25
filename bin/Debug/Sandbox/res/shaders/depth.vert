#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

uniform mat4 u_model;
uniform mat4 u_projection_view;

out vec2 frag_uv;

void main()
{
    frag_uv = aTex;

    gl_Position = u_projection_view * u_model * vec4(aPos, 1.0);
}