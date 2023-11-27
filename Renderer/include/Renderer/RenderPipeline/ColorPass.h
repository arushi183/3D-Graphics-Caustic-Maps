#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Scene.h"

namespace renderer
{

	class ColorPass : public RenderPass
	{
	public:
		ColorPass(graphics::Scene* scene, Renderer* renderer);
		~ColorPass();

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
	private:
		graphics::Scene* m_scene;
		graphics::Shader* m_lineShader;
		int m_viewportHeight, m_viewportWidth;
		unsigned int m_ColorFBO;
		graphics::Texture* m_colorTexture;
		graphics::Texture* m_depthTexture;

		std::unordered_map<graphics::Light*, graphics::Texture*> m_shadowStruct;
	};

}