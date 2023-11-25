#pragma once

namespace renderer
{
	class Renderer;

	class RenderPass
	{
	public:
		RenderPass(Renderer* renderer);
		virtual void preRender() = 0;
		virtual void render() = 0;
		virtual void postRender() = 0;

		virtual void setInputs(RenderPass* pass) = 0;
		virtual void getOutputs(void* inputStruct) = 0;
	protected:
		Renderer* m_renderer;
	};
}