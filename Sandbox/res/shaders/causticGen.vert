#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out vec3 vertexCol;
out float waterDepth;
out float depth;
out vec3 oldPos;
out vec3 newPos;

uniform float u_time;
uniform vec3 u_wave[32];

uniform mat4 u_projection;
uniform mat4 u_view;
uniform vec3 u_cameraPos;
uniform float m_near;
uniform float m_far;

uniform sampler2D t_environmentDepth;

const float deltaEnvTexture = 1.0 / 1024.0;
const int MAX_ITERATIONS = 50;

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

vec3 getWorldPos(vec3 ndc)
{
    vec4 projPos = inverse(u_projection * u_view) * vec4(ndc, 1.0);
    return projPos.xyz/projPos.w;
}

vec4 environmentSample(vec2 coords)
{
    float environmentDepthNL = texture(t_environmentDepth, coords).r;
	environmentDepthNL = environmentDepthNL * 2.0 - 1.0;
	float environmentDepth = ((2.0 * m_near * m_far) / (m_far + m_near - environmentDepthNL * (m_far - m_near)));

    vec4 projPos = inverse(u_projection * u_view) * vec4((coords.xy * 2.0) - 1.0, -1.0, 1.0);
    vec3 worldPos = projPos.xyz/projPos.w;

    vec3 environmentPos = vec3(worldPos.x, environmentDepth, worldPos.z) - u_cameraPos;
    return vec4(environmentPos, environmentDepth);
}

void main()
{

    gl_Position = vec4(aPos, 1.0);

    vec3 worldPos = getWorldPos(aPos);
    // Ray from lightwavefront to surface of water -> waterPos
    // normal at the ray/water intersection = waterNormal
    vec3 waterPos = worldPos;
    waterPos.y = 0;
    
    vec3 waveInfo = calculateHeight(waterPos);
    waterPos.y += waveInfo.x;

    vec3 binormal = normalize(vec3(1,waveInfo.y,0));
    vec3 tangent = normalize(vec3(0,waveInfo.z,1));

    vec3 waterNormal = normalize(cross(binormal, tangent));

    oldPos = waterPos;

    vec4 projectedWaterPos = u_projection * u_view * vec4(waterPos, 1.0);

    vec2 currentPosition = projectedWaterPos.xy;
    vec2 coords = 0.5 + 0.5 * currentPosition;

    float refractionRatio = 1.0 / 1.33;
    vec3 refracted = refract(vec3(0,-1,0), -waterNormal, refractionRatio);
    vec4 projectedRefractionVector = u_projection * u_view * vec4(refracted, 1.0);

    vec3 refractedDir = projectedRefractionVector.xyz;

    waterDepth = 0.5 + 0.5 * projectedWaterPos.z / projectedWaterPos.w;
    float currentDepth = projectedWaterPos.z;
    
    vec4 environment = environmentSample(coords);

    float factor = deltaEnvTexture / length(refractedDir.xy);

    vec2 deltaDirection = refractedDir.xy * factor;
    float deltaDepth = refractedDir.z * factor;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        // Move the coords in the direction of the refraction
        currentPosition += deltaDirection;
        currentDepth += deltaDepth;
        
        // End of loop condition: The ray has hit the environment
        if (environment.w <= currentDepth) {
            environment.xyz = vec3(1,0,0);
            break;
        }
        
        environment = environmentSample(0.5 + 0.5 * currentPosition);
    }
    
    newPos = environment.xyz;
    vec4 projectedEnvPosition = u_projection * u_view * vec4(newPos, 1.0);
    depth = 0.5 + 0.5 * projectedEnvPosition.z / projectedEnvPosition.w;
}