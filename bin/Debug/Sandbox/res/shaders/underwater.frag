#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 frag_pos;
    vec3 frag_normal;
    vec2 frag_uv;
    vec4 frag_lightTransform;
    vec4 frag_causticTransform;
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
uniform vec3 u_cameraPos;

uniform sampler2D t_directionalShadowMap;
uniform sampler2D t_diffuseTex;
uniform sampler2D t_causticTexture;

float CausticCalc(vec4 fragPosCausticTransform)
{
	vec3 projCoords = fragPosCausticTransform.xyz/ fragPosCausticTransform.w;
	projCoords = (projCoords * 0.5) + 0.5;

	return texture(t_causticTexture, projCoords.xy).r;
}

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
	
	vec4 baseColor = texture(t_diffuseTex, fs_in.frag_uv);
	float causticFactor = CausticCalc(fs_in.frag_causticTransform);


	vec4 ambientColor = vec4(u_directionalLight.base.color, 1.0) * u_directionalLight.base.ambientIntensity;

	float diffuseFactor = max(dot(normalize(fs_in.frag_normal), normalize(u_directionalLight.direction)), 0.0f);
	vec4 diffuseColor = vec4(u_directionalLight.base.color, 1.0f) *  u_directionalLight.base.diffuseIntensity * diffuseFactor;

	vec3 fragToEye = normalize(u_cameraPos - fs_in.frag_pos);
	vec3 reflected = normalize(reflect(u_directionalLight.direction, normalize(fs_in.frag_normal)));
	
	vec4 specularColor = vec4(0,0,0,0);
	float specularFactor = dot(fragToEye, reflected);
	specularFactor = pow(specularFactor, 32);
	if(specularFactor > 0)
		specularColor = vec4(u_directionalLight.base.color, 1.0f) * specularFactor;

	float shadowFactor = shadowCalc(fs_in.frag_lightTransform);
	if(shadowFactor < 0.9)
		baseColor = vec4(mix(baseColor.rgb, u_directionalLight.base.color, causticFactor), 1.0);

	FragColor = vec4(baseColor.xyz * (ambientColor.xyz + ((1.0 - shadowFactor) * (diffuseColor.xyz + specularColor.xyz))), baseColor.w);
}