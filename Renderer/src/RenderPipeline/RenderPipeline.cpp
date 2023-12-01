#include "Renderer/RenderPipeline/RenderPipeline.h"

namespace renderer
{

	RenderPipeline::RenderPipeline()
	{
	}

	RenderPipeline::~RenderPipeline()
	{
		for (auto& pass : m_passes)
			delete(pass);
	}

	void RenderPipeline::addPass(RenderPass* pass)
	{
		m_passes.push_back(pass);
	}

	void RenderPipeline::renderPipeline()
	{
		for (int i = 0; i < m_passes.size(); i++)
		{
			if (i > 0)
				m_passes[i]->setInputs(m_passes[i - 1]);
			m_passes[i]->preRender();
			m_passes[i]->render();
			m_passes[i]->postRender();
		}
	}

}