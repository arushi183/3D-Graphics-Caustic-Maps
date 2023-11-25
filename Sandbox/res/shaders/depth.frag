#version 330 core

in vec2 frag_uv;

uniform sampler2D t_alphaTex;

void main()
{
	vec4 baseColor = texture(t_alphaTex, frag_uv);
	if(baseColor.a <= 0.1)
		discard;
}