#pragma once

#include <vector>

#include "RenderPass.h"

namespace renderer
{

	class RenderPipeline
	{
	public:
		RenderPipeline();
		~RenderPipeline();

		void addPass(RenderPass* pass);

		void renderPipeline();
	private:
		std::vector<RenderPass*> m_passes;
	};

}