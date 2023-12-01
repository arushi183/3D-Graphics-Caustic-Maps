#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

uniform float u_time;
uniform vec3 u_wave[32];

vec3 calculateHeight(vec3 position)
{
    vec3 answer = vec3(0,0,0);

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


        answer.x += wave;

        answer.y += d.x * dx; 
        answer.z += d.y * dx;

        p.xz += d * -dx * amplitude;

        amplitudeSum += amplitude;

        frequency *= 1.18f;
        amplitude *= 0.82f;
    }

    answer = answer / amplitudeSum;

    return answer * 0.5;
}

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_directionalLightTransform;

out VS_OUT
{
    vec3 frag_pos;
    vec3 frag_normal;
    vec2 frag_uv;
    vec4 frag_lightTransform;
} vs_out;

void main()
{
    vec3 pos = vec3(u_model[3][0], u_model[3][1], u_model[3][2]);
    vec3 waveInfo = calculateHeight(pos);
    pos.y += waveInfo.x;

    mat4 model = u_model;
    model[3][0] = pos.x;
    model[3][1] = pos.y;
    model[3][2] = pos.z;

    vs_out.frag_pos = vec3(model * vec4(aPos, 1.0));
    vs_out.frag_normal = mat3(transpose(inverse(model))) * aNorm;
    vs_out.frag_uv = aTex;
    vs_out.frag_lightTransform = u_directionalLightTransform * vec4(vs_out.frag_pos, 1.0);

    gl_Position = u_projection * u_view * vec4(vs_out.frag_pos, 1.0);
}