#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Scene.h"

namespace renderer
{

	class ScreenPass : public RenderPass
	{
	public:
		ScreenPass(graphics::Scene* scene, Renderer* renderer, graphics::Shader* screenShader);
		~ScreenPass();

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
	private:
		graphics::Scene* m_scene;
		graphics::Shader* m_screenShader;
		int m_viewportHeight, m_viewportWidth;
		graphics::Texture* m_colorTexture;
		graphics::Texture* m_depthTexture;
		graphics::Texture* m_shadowTexture;

		glm::mat4 m_lightTransform;

		unsigned int m_VAO, m_VBO, m_IBO;
	};
}