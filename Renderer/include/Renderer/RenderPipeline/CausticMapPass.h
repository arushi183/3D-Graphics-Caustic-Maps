#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Texture.h"
#include "Renderer/Graphics/Scene.h"

#include <vector>


namespace renderer
{

	class CausticMapPass : public RenderPass
	{
	public:
		CausticMapPass(graphics::Scene* scene, Renderer* renderer);

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
	private:
		graphics::Scene* m_scene;
		graphics::Shader* m_causticMapShader;
		graphics::Texture* m_causticTexture;
		graphics::Texture* m_depthTexture;
		unsigned int m_causticFBO;
		glm::mat4 m_projection;
	};

}