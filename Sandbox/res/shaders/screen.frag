#version 330 core
out vec4 FragColor;

in vec2 frag_uv;

uniform sampler2D t_mainTex;
uniform sampler2D t_depthTex;

uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_cameraPos;
uniform float m_halfScreenWidth;
uniform float m_halfScreenHeight;

//vec3 getRayFromScreenSpace(vec2 pos)
//{
//    float x = (2*pos.x) - 1;
//    float y = (2*pos.y) - 1;
//    float z = 1.0f;
//    vec3 ray_nds = vec3(x,y,z);
//
//	vec4 ray_clip = vec4(ray_nds.xy, -1.0, 1.0);
//
//	vec4 ray_eye = inverse(u_projection) * ray_clip;
//	ray_eye = vec4(ray_eye.xy, -1.0, 0.0);
//
//	vec3 ray_wor = (inverse(u_view) * ray_eye).xyz;
//
//	ray_wor = normalize(ray_wor);
//	return ray_wor;
//}

void main()
{
	//vec3 ray = getRayFromScreenSpace(frag_uv);
	//vec3 rayO = u_cameraPos;
	//vec3 rayE = rayO + ray * texture(t_depthTex, frag_uv).r;

	vec3 col = vec3(texture(t_mainTex, frag_uv));
	FragColor = vec4(col.rgb, 1.0);
}