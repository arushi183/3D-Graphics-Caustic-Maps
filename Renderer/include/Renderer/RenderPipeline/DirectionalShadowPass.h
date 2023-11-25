#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Texture.h"
#include "Renderer/Graphics/Scene.h"

#include <vector>


namespace renderer
{

	class DirectionalShadowPass : public RenderPass
	{
	public:
		DirectionalShadowPass(graphics::Scene* scene, Renderer* renderer);

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
	private:
		graphics::Scene* m_scene;
		graphics::Shader* m_depthMapShader;
		std::unordered_map<graphics::Texture*, graphics::Light*> m_shadowMaps;
		std::vector<unsigned int> m_shadowFBOs;
	};

}