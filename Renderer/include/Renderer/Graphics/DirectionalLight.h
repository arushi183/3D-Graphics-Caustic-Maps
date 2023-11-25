#pragma once

#include "Light.h"
#include "Renderable.h"

namespace graphics
{
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

	class Texture;

	class DirectionalLight: public Light
	{
	public:
		DirectionalLight(bool castShadows = false);
		~DirectionalLight();

		int getType() override;

		const glm::mat4 getLightTransform();
		const glm::vec3 getForward();
		void getShadowObject(unsigned int& shadowFBO, Texture*& shadowTexture) override;

		void render(Shader* shader, renderer::Renderer* render) override;
	public:
		Tranformation transform;
	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;
		Texture* m_shadowMap;
		unsigned int m_shadowFBO;
	};
}