#version 330 core
out vec4 FragColor;

in vec2 frag_uv;

uniform sampler2D t_mainTex;
uniform sampler2D t_depthTex;
uniform sampler2D t_shadowMapTex;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_directionalLightTransform;

uniform float m_near;
uniform float m_far;

uniform vec3 u_cameraPos;

float shadowCalc(vec4 fragPosLightTransform)
{
	vec3 projCoords = fragPosLightTransform.xyz/ fragPosLightTransform.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float currentDepth = projCoords.z;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(t_shadowMapTex, 0);

	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			float PCFDepth = texture(t_shadowMapTex, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth > PCFDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	if(projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;
}

vec2 rayBoxDist(vec3 boundsMin, vec3 boundsMax, vec3 rayOrigin, vec3 rayDir)
{
	vec3 t0 = (boundsMin - rayOrigin) / rayDir;
	vec3 t1 = (boundsMax - rayOrigin) / rayDir;
	vec3 tmin = min(t0, t1);
	vec3 tmax = max(t0, t1);

	float dstA = max(max(tmin.x, tmin.y), tmin.z);
	float dstB = min(tmax.x, min(tmax.y, tmax.z));

	float dstToBox = max(0, dstA);
	float dstInsideBox = max(0, dstB - dstToBox);
	return vec2(dstToBox, dstInsideBox);
}

vec3 getRay(vec2 pos)
{
	float x = (2.0 * pos.x) - 1.0;
	float y = (2.0 * pos.y) - 1.0;
	
	vec4 ray_clip = vec4(x, y, -1.0, 1.0);
	vec4 ray_eye = inverse(u_projection) * ray_clip;
	ray_eye = vec4(ray_eye.xy, -1.0, 0.0);

	vec3 ray_wor = (inverse(u_view) * ray_eye).xyz;
	return normalize(ray_wor);
}

float sampleDensity(vec3 position)
{
	float shadowFactor = shadowCalc(u_directionalLightTransform * vec4(position, 1.0));
	return (1 - shadowFactor);
}

void main()
{
	vec3 col = vec3(texture(t_mainTex, frag_uv));

	float nonLinearDepth = texture(t_depthTex, frag_uv).r;
	nonLinearDepth = nonLinearDepth * 2.0 - 1.0;
	float depth = ((2.0 * m_near * m_far) / (m_far + m_near - nonLinearDepth * (m_far - m_near)));
	

	vec3 rayOrigin = u_cameraPos;
	vec3 rayDir = getRay(frag_uv);

	vec2 rayBoxInfo = rayBoxDist(vec3(-25,-20,-25), vec3(25,0,25), rayOrigin, rayDir);
	float dstToBox = rayBoxInfo.x;
	float dstInsideBox = rayBoxInfo.y;

	float dstTravelled = 0;
	float stepSize = dstInsideBox / 128;
	float dstLimit = min(depth - dstToBox, dstInsideBox);

	float totalDensity = 0;
	vec3 underwaterColor = vec3(0.000, 0.533, 0.666);
	int steps = 1;
	while(dstTravelled < dstLimit)
	{
		vec3 rayPos = rayOrigin + rayDir * (dstToBox + dstTravelled);
		float shadowFactor = sampleDensity(rayPos);
		underwaterColor += mix(vec3(0.0, 0.0, 0.0), vec3(0.000, 0.533, 0.666), shadowFactor);
		totalDensity += 0.06 * stepSize;
		dstTravelled += stepSize;
		steps += 1;
	}
	
	underwaterColor /= steps;

	float transmittance = exp(-totalDensity);

	col = mix(underwaterColor, col, transmittance);

	//vec3 Worldpos = u_cameraPos + (getRay(frag_uv) * linearDepth);

	FragColor = vec4(col.rgb, 1.0);
}