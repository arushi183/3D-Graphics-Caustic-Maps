#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/ColorPass.h"
#include "Renderer/Graphics/ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace renderer
{

	ColorPass::ColorPass(graphics::Scene* scene, Renderer* renderer)
		:RenderPass(renderer), m_scene(scene)
	{
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		m_viewportWidth = m_viewport[2];
		m_viewportHeight = m_viewport[3];
	}

	void ColorPass::preRender()
	{
		glViewport(0, 0, m_viewportWidth, m_viewportHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ColorPass::render()
	{
		for (auto& set : m_scene->meshList)
		{
			graphics::Shader* shader = set.first;
			shader->bind();

			shader->setUnifromMat4f("u_projection", m_scene->mainCamera->getProjectionMatrix());
			shader->setUnifromMat4f("u_view", m_scene->mainCamera->getViewMatrix());
			// Set uniforms like lights
			// and inputs

			for (graphics::Renderable* mesh : set.second)
				mesh->render(*m_renderer);
		}
	}

	void ColorPass::postRender() {}

	void ColorPass::setInputs()
	{
	}

	void ColorPass::getOutputs()
	{
	}

}