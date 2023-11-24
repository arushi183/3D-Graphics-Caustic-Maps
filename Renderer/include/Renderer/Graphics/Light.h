#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics
{

	class Light
	{
	protected:
		Light() : color({1,1,1}), ambientIntensity(1), diffuseIntensity(1) {};
	public:
		glm::vec3 color;
		float ambientIntensity;
		float diffuseIntensity;
	};

}