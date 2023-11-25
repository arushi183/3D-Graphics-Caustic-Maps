#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/DirectionalShadowPass.h"
#include "Renderer/Graphics/ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Graphics/DirectionalLight.h"

namespace renderer
{

	DirectionalShadowPass::DirectionalShadowPass(graphics::Scene* scene, Renderer* renderer)
		:RenderPass(renderer), m_scene(scene), m_depthMapShader(nullptr)
	{
		m_depthMapShader = graphics::ShaderManager::getInstance()->loadShader("res/shaders/depth.vert", "res/shaders/depth.frag");
	}

	void DirectionalShadowPass::preRender()
	{
		glCullFace(GL_FRONT);
		m_depthMapShader->bind();
		m_depthMapShader->setUniformInt("t_alphaTex", 1);
		m_shadowMaps.clear();
		m_shadowFBOs.clear();
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		for (auto& l : m_scene->lightList) {
			unsigned int shadowFBO;
			graphics::Texture* shadowTexture;
			l->getShadowObject(shadowFBO, shadowTexture);
			if (shadowFBO != 0 && l->getType() == DIRECTIONAL_LIGHT)
			{
				m_shadowFBOs.push_back(shadowFBO);
				m_shadowMaps[shadowTexture] = l;

				glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
				glClear(GL_DEPTH_BUFFER_BIT);
			}
		}
	}

	void DirectionalShadowPass::render()
	{
		graphics::DirectionalLight* sunLight = nullptr;
		for (auto& l : m_scene->lightList) {
			if (l->getType() == DIRECTIONAL_LIGHT)
			{
				unsigned int FBO;
				graphics::Texture* shadowTexture;
				l->getShadowObject(FBO, shadowTexture);
				if (FBO != 0)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);
					sunLight = (graphics::DirectionalLight*)l;
					m_depthMapShader->setUnifromMat4f("u_projection_view", glm::value_ptr(sunLight->getLightTransform()));
					for (auto& set : m_scene->meshList)
					{
						for (graphics::Renderable* mesh : set.second)
							mesh->render(*m_renderer, m_depthMapShader);
					}
				}
			}
		}
	}

	void DirectionalShadowPass::postRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
	}

	void DirectionalShadowPass::setInputs(RenderPass* pass)
	{
	}

	void DirectionalShadowPass::getOutputs(void* inputStruct)
	{
		std::unordered_map<graphics::Light*, graphics::Texture*> *shadowStruct = (std::unordered_map<graphics::Light*, graphics::Texture*>*)inputStruct;
		(*shadowStruct).clear();
		for (auto& shmap : m_shadowMaps)
		{
			(*shadowStruct)[shmap.second] = shmap.first;
		}
	}

}