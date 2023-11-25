#version 330 core
out vec4 FragColor;

in vec2 frag_uv;

uniform sampler2D t_mainTex;

void main()
{
	vec3 col = vec3(texture(t_mainTex, frag_uv));
	FragColor = vec4(col.rgb, 1.0);
}