#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_directionalLightTransform;
uniform mat4 u_causticView;
uniform mat4 u_causticProj;

out VS_OUT
{
    vec3 frag_pos;
    vec3 frag_normal;
    vec2 frag_uv;
    vec4 frag_lightTransform;
    vec4 frag_causticTransform;
} vs_out;

void main()
{
    vs_out.frag_pos = vec3(u_model * vec4(aPos, 1.0));
    vs_out.frag_normal = mat3(transpose(inverse(u_model))) * aNorm;
    vs_out.frag_uv = aTex;
    vs_out.frag_lightTransform = u_directionalLightTransform * vec4(vs_out.frag_pos, 1.0);
    vs_out.frag_causticTransform = u_causticProj * u_causticView * vec4(vs_out.frag_pos, 1.0);

    gl_Position = u_projection * u_view * vec4(vs_out.frag_pos, 1.0);
}