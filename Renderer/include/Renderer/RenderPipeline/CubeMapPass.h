#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Scene.h"

namespace renderer
{

	class CubeMapPass : public RenderPass
	{
	public:
		CubeMapPass(graphics::Scene* scene, Renderer* renderer);
		~CubeMapPass();

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs(RenderPass* pass) override;
		void getOutputs(void* inputStruct) override;
	private:
		graphics::Scene* m_scene;
		std::vector<graphics::Renderable*> m_DrawList;
		glm::mat4 m_projection;
	};

}