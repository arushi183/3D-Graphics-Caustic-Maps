#version 330 core
layout (location = 0) in vec3 aPos;
uniform vec3 light;

uniform sampler2D env;
uniform float u_time;
uniform vec3 u_wave[32];

out float waterDepth;
out float depth;

out vec3 oldPosition;
out vec3 newPosition;

const float eta = 0.7504;
const float deltaEnvTexture = 1.0/720.0;

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

void main() {
    vec3 waterPosition = vec3(u_model * vec4(aPos, 1.0));
    vec3 wave = calculateHeight(pos);
    waterPosition.y += wave.x;

    vec3 binormal = normalize(vec3(1,wave.y,0));
    vec3 tangent = normalize(vec3(0,wave.z,1));

    vec3 waterNormal = normalize(cross(binormal, tangent));


  // This is the initial position: the ray starting point
  oldPosition = waterPosition;

  // Compute water coordinates in the screen space
  vec4 projectedWaterPosition = projectionMatrix * viewMatrix * vec4(waterPosition, 1.);

  vec2 currentPosition = projectedWaterPosition.xy;
  vec2 coords = 0.5 + 0.5 * currentPosition;

  vec3 refracted = refract(light, waterNormal, eta);
  vec4 projectedRefractionVector = projectionMatrix * viewMatrix * vec4(refracted, 1.);

  vec3 refractedDirection = projectedRefractionVector.xyz;

  waterDepth = 0.5 + 0.5 * projectedWaterPosition.z / projectedWaterPosition.w;
  float currentDepth = projectedWaterPosition.z;
  vec4 environment = texture2D(env, coords);

  // This factor will scale the delta parameters so that we move from one pixel to the other in the env map
  float factor = deltaEnvTexture / length(refractedDirection.xy);

  vec2 deltaDirection = refractedDirection.xy * factor;
  float deltaDepth = refractedDirection.z * factor;

  for (int i = 0; i < MAX_ITERATIONS; i++) {
    // Move the coords in the direction of the refraction
    currentPosition += deltaDirection;
    currentDepth += deltaDepth;

    // End of loop condition: The ray has hit the environment
    if (environment.w <= currentDepth) {
      break;
    }

    environment = texture2D(env, 0.5 + 0.5 * currentPosition);
  }

  newPosition = environment.xyz;

  vec4 projectedEnvPosition = projectionMatrix * viewMatrix * vec4(newPosition, 1.0);
  depth = 0.5 + 0.5 * projectedEnvPosition.z / projectedEnvPosition.w;

  gl_Position = projectedEnvPosition;
}