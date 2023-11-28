#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

out vec2 frag_uv;
out vec4 worldPosition;
out float gl_FragDepth;

uniform mat4 u_model;
uniform mat4 u_projection_view;

void main()
{
    frag_uv = aTex;

    worldPosition = u_model * vec4(aPos, 1.0);

    // Project vertex in screen coordinates
    gl_Position = u_projection_view * worldPosition;
}
