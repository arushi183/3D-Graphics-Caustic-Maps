#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 frag_pos;
    vec3 frag_normal;
    vec2 frag_uv;
    vec4 frag_lightTransform;
} fs_in;

struct LightBase
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	LightBase base;
	vec3 direction;
};

uniform DirectionalLight u_directionalLight;
uniform sampler2D t_directionalShadowMap;
uniform samplerCube t_cubeMap;

uniform vec3 u_cameraPos;
uniform vec4 u_diffuseColor;

float shadowCalc(vec4 fragPosLightTransform)
{
	vec3 projCoords = fragPosLightTransform.xyz/ fragPosLightTransform.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float currentDepth = projCoords.z;
	float closestDepth = texture(t_directionalShadowMap, projCoords.xy).r;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(t_directionalShadowMap, 0);

	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			float PCFDepth = texture(t_directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth > PCFDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	if(projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;
}

void main()
{
	vec3 normal = normalize(fs_in.frag_normal);
	if(dot(normal, normalize(u_directionalLight.direction)) < 0)
		normal = -normal;

	float refraction_ratio = 1.00 / 1.33;
	vec3 fragToEye = normalize(fs_in.frag_pos-u_cameraPos);
	float fresnel = pow((1-max(dot(normal, fragToEye), 0.0f)), 1);
	vec3 refractedRay = normalize(refract(fragToEye, -normal, refraction_ratio));
	vec3 reflectedRay = normalize(reflect(fragToEye, -normal));

	vec3 refractionColor = texture(t_cubeMap, refractedRay).xyz;
	vec3 reflectionColor = texture(t_cubeMap, reflectedRay).xyz;
	
	vec3 baseColor = mix(refractionColor, u_diffuseColor.xyz, min(fresnel+0.2,1));
	baseColor = mix(baseColor, reflectionColor, 0.25);

	vec4 ambientColor = vec4(u_directionalLight.base.color, 1.0) * u_directionalLight.base.ambientIntensity;

	float diffuseFactor = max(dot(normal, normalize(u_directionalLight.direction)), 0.0f);
	vec4 diffuseColor = vec4(u_directionalLight.base.color, 1.0f) *  u_directionalLight.base.diffuseIntensity * diffuseFactor;

	fragToEye = normalize(u_cameraPos - fs_in.frag_pos);
	vec3 reflected = normalize(reflect(u_directionalLight.direction, normal));
	
	vec4 specularColor = vec4(0,0,0,0);
	float specularFactor = dot(fragToEye, reflected);
	specularFactor = pow(specularFactor, 32);
	if(specularFactor > 0)
		specularColor = vec4(u_directionalLight.base.color, 1.0f) * specularFactor;

	FragColor = vec4(baseColor.xyz * (ambientColor.xyz + (diffuseColor.xyz + specularColor.xyz)), 1.0);
}