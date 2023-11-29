#pragma once
#include "RenderPass.h"
#include "Renderer/Graphics/Texture.h"
#include "Renderer/Graphics/Scene.h"
#include "Renderer/Graphics/Model.h"

#include <vector>


namespace renderer
{

	class CausticMapPass : public RenderPass
	{
	public:
		CausticMapPass(graphics::Scene* scene, Renderer* renderer, graphics::Material* causticMaterial);

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
		graphics::Scene* m_scene;
		graphics::Shader* m_causticMapShader;
		graphics::Texture* m_causticTexture;
		unsigned int m_causticFBO;
		unsigned int m_causticRBO;
		glm::mat4 m_projection;
		graphics::Mesh* m_quadMap;
		graphics::Material* m_causticMaterial;
	};

}
