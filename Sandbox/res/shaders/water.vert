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
    vec3 output = vec3(0,0,0);

    float amplitude = 1.0f;
    float frequency = 0.3f;

    vec3 p = position;
    float amplitudeSum = 0;

    for(int i=0; i<32; i++)
    {
        vec2 d = normalize(u_wave[i].xy);

        float x = dot(d, p.xz) * frequency + u_time;
        float wave = amplitude * exp(sin(x));
        float dx = wave * cos(x);


        output.x += wave;

        output.y += d.x * dx; 
        output.z += d.y * dx;

        p.xz += d * -dx * amplitude;

        amplitudeSum += amplitude;

        frequency *= 1.18f;
        amplitude *= 0.82f;
    }

    output = output / amplitudeSum;

    return output * 0.5;
}

void main()
{
    vec3 pos = vec3(u_model * vec4(aPos, 1.0));
    vec3 wave = calculateHeight(pos);
    pos.y += wave.x;

    vec3 binormal = normalize(vec3(1,wave.y,0));
    vec3 tangent = normalize(vec3(0,wave.z,1));

    vec3 normal = normalize(cross(binormal, tangent));

    vs_out.frag_pos = pos;
    vs_out.frag_normal = mat3(transpose(inverse(u_model))) * normal;
    vs_out.frag_lightTransform = u_directionalLightTransform * vec4(vs_out.frag_pos, 1.0);

    gl_Position = u_projection * u_view * vec4(vs_out.frag_pos, 1.0);
}