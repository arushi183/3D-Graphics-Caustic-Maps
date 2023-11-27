#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Texture.h"
#include "Renderer/Graphics/Scene.h"

#include <vector>


namespace renderer
{

	class EnvironmentMapPass : public RenderPass
	{
	public:
		EnvironmentMapPass(graphics::Scene* scene, Renderer* renderer);

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
	private:
		graphics::Scene* m_scene;
		graphics::Shader* m_environmentMapShader;
		std::unordered_map<graphics::Texture*, graphics::Light*> m_shadowMaps;
		std::vector<unsigned int> m_shadowFBOs;
		glm::mat4 m_projection;
		std::vector<graphics::Renderable*> m_DrawList;
	};

}