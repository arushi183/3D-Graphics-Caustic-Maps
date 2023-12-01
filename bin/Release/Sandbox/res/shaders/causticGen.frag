#version 330 core

out vec4 FragColor;

in vec3 vertexCol;
in float waterDepth;
in float depth;
in vec3 oldPos;
in vec3 newPos;

const float causticFactor = 0.15;

void main()
{
	float causticIntensity = 0.0;

	if(depth >= waterDepth)
	{
		float oldArea = length(dFdx(oldPos)) * length(dFdy(oldPos));
		float newArea = length(dFdx(newPos)) * length(dFdy(newPos));

		float ratio;

		if(newArea == 0.0)
		{
			ratio = 2.0e+20;
		}
		else
		{
			ratio = oldArea / newArea;
		}

		causticIntensity = causticFactor * ratio;
	}

	FragColor = vec4(causticIntensity, causticIntensity, causticIntensity, 1.0);
}