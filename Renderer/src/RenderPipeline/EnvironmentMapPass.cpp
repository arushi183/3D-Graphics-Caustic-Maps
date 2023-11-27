#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/EnvironmentMapPass.h"
#include "Renderer/Graphics/ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Graphics/DirectionalLight.h"

namespace renderer
{

	EnvironmentMapPass::EnvironmentMapPass(graphics::Scene* scene, Renderer* renderer)
		:RenderPass(renderer), m_scene(scene), m_environmentMapShader(nullptr)
	{
		m_environmentMapShader = graphics::ShaderManager::getInstance()->loadShader("res/shaders/environmentmap.vert", "res/shaders/environmentmap.frag");
	}

	void EnvironmentMapPass::preRender()
	{
		glCullFace(GL_FRONT);
		m_environmentMapShader->bind();
		m_environmentMapShader->setUniformInt("t_alphaTex", 1);
		
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		
	}

	void EnvironmentMapPass::render()
	{
		/*m_screenShader->bind();
		m_colorTexture->useTexture(0);
		m_renderer->render(m_VAO, 6);*/

		//m_environmentMapShader->setUnifromMat4f("u_projection_view", glm::value_ptr(sunLight->getLightTransform()));
		for (auto& set : m_scene->meshList)
		{
			for (graphics::Renderable* mesh : set.second)
			{
				if (mesh->getMaterialRef()->reciveShadows)
					mesh->render(*m_renderer, m_environmentMapShader);
			}
		}
	}

	void EnvironmentMapPass::postRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
	}

	void EnvironmentMapPass::setInputs(RenderPass* pass)
	{
	}

	void EnvironmentMapPass::getOutputs(void* inputStruct)
	{
		std::unordered_map<graphics::Light*, graphics::Texture*>* shadowStruct = (std::unordered_map<graphics::Light*, graphics::Texture*>*)inputStruct;
		(*shadowStruct).clear();
		for (auto& shmap : m_shadowMaps)
		{
			(*shadowStruct)[shmap.second] = shmap.first;
		}
	}

}