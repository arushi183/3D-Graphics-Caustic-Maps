#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace renderer
{
	class Renderer;
}

namespace graphics
{

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

	class Shader;
	class Texture;

	class Light
	{
	protected:
		Light() : color({1,1,1}), ambientIntensity(1), diffuseIntensity(1), m_VAO(0), m_VBO(0) {};
		unsigned int m_VAO, m_VBO;
	public:
		virtual int getType() { return -1; }

		virtual void render(Shader* shader, renderer::Renderer* render) = 0;
		virtual void getShadowObject(unsigned int& shadowFBO, Texture*& shadowTexture) = 0;
	public:
		glm::vec3 color;
		float ambientIntensity;
		float diffuseIntensity;
	};

}