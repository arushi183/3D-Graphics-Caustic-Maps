#pragma once

#include "RenderPass.h"
#include "Renderer/Graphics/Scene.h"

namespace renderer
{

	class ColorPass : public RenderPass
	{
	public:
		ColorPass(graphics::Scene* scene, Renderer* renderer);

		void preRender() override;
		void render() override;
		void postRender() override;

		void setInputs() override;
		void getOutputs() override;
	private:
		graphics::Scene* m_scene;
		int m_viewportHeight, m_viewportWidth;
	};

}