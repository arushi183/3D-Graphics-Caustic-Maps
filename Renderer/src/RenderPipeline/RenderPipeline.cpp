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
		for (RenderPass* pass : m_passes)
		{
			//Add Logic to keep track of inputs and outputs
			//pass->setInputs();
			pass->preRender();
			pass->render();
			pass->postRender();
			//pass->getOutputs();
		}
	}

}