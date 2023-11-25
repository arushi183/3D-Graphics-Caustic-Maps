#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Scene.h"

namespace renderer
{

	class ScreenPass : public RenderPass
	{
	public:
		ScreenPass(Renderer* renderer, graphics::Shader* screenShader);
		~ScreenPass();

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
	private:
		graphics::Shader* m_screenShader;
		int m_viewportHeight, m_viewportWidth;
		graphics::Texture* m_colorTexture;

		unsigned int m_VAO, m_VBO, m_IBO;
	};
}