#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_directionalLightTransform;

uniform float u_time;
uniform vec3 u_wave[32];

out VS_OUT
{
    vec3 frag_pos;
    vec3 frag_normal;
    vec2 frag_uv;
    vec4 frag_lightTransform;
} vs_out;

vec3 calculateHeight(vec3 position)
{
    vec3 output = vec3(0,1,1);
    output.x += sin(dot(u_wave[0].xy,position.xz) * 1000 + u_time);
    //vec2 dir = vec2(0,1);
    //output.x = sin(dot(dir.xy, position.xz) + u_time);
    //output.y = 1 * cos(dot(dir.xy, position.xz) + u_time);
    //output.z = 1 * cos(dot(dir.xy, position.xz) + u_time);

    //float amplitude = 0.3f;
    //float frequency = 1.0f;
    //for(int i=0; i<32; i++)
    //{
    //    output.x += amplitude * sin(dot(u_wave[i].xy,position.xz) * frequency + u_time);
    //    frequency *= 7.18f;
    //    amplitude *= 0.282f;
    //}

    return output;
}

void main()
{
    vec3 pos = vec3(u_model * vec4(aPos, 1.0));
    vec3 wave = calculateHeight(pos);
    pos.y += wave.x;

    vec3 binormal = vec3(wave.y,0,0);
    vec3 tangent = vec3(0,0,wave.z);

    vec3 normal = normalize(cross(binormal, tangent));

    vs_out.frag_pos = pos;
    vs_out.frag_normal = mat3(transpose(inverse(u_model))) * normal;
    vs_out.frag_lightTransform = u_directionalLightTransform * vec4(vs_out.frag_pos, 1.0);

    gl_Position = u_projection * u_view * vec4(vs_out.frag_pos, 1.0);
}