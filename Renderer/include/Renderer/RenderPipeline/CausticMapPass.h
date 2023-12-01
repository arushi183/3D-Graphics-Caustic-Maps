#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Texture.h"
#include "Renderer/Graphics/Scene.h"

namespace graphics
{
	class Mesh;
}

namespace renderer
{

	class CausticMapPass : public RenderPass
	{
	public:
		CausticMapPass(graphics::Scene* scene, Renderer* renderer, graphics::Material* causticGenMaterial);
		~CausticMapPass();

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
	private:
		graphics::Scene* m_scene;
		glm::mat4 m_projection;
		glm::mat4 m_view;

		graphics::Shader* m_depthShader;
		graphics::Texture* m_EnvdepthTexture;
		unsigned int m_environmentFBO;

		graphics::Material* m_causticGenMaterial;
		graphics::Mesh* m_lightWavefront;
		graphics::Texture* m_CausticTexture;
		unsigned int m_causticFBO, m_causticRBO;
	};
}