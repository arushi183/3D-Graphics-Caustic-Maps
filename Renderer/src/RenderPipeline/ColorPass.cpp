#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/ColorPass.h"
#include "Renderer/RenderPipeline/DirectionalShadowPass.h"
#include "Renderer/Graphics/ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Graphics/DirectionalLight.h"

namespace renderer
{

	ColorPass::ColorPass(graphics::Scene* scene, Renderer* renderer)
		:RenderPass(renderer), m_scene(scene)
	{
		m_lineShader = graphics::ShaderManager::getInstance()->loadShader("res/shaders/unshaded.vert", "res/shaders/unshaded.frag");
		renderer->getUpdatedWindow(m_viewportWidth, m_viewportHeight);

		m_colorTexture = new graphics::Texture(m_viewportWidth, m_viewportHeight, 3);
		m_depthTexture = new graphics::Texture(m_viewportWidth, m_viewportHeight, 1);
		glGenFramebuffers(1, &m_ColorFBO);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ColorFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture->getTextureID(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->getTextureID(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ColorPass::~ColorPass()
	{
		if (m_ColorFBO != 0)
		{
			delete(m_colorTexture);
			delete(m_depthTexture);
			glDeleteFramebuffers(1, &m_ColorFBO);
		}
	}

	void ColorPass::preRender()
	{
		m_renderer->getUpdatedWindow(m_viewportWidth, m_viewportHeight);
		glViewport(0, 0, m_viewportWidth, m_viewportHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ColorFBO);
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

			shader->setUniformVec3("u_cameraPos", glm::value_ptr(m_scene->mainCamera->transform.position));
			shader->setUniformInt("t_diffuseTex", 1);
			

			int point_light_index = 0;
			int spot_light_index = 0;
			for (size_t i = 0; i < m_scene->lightList.size(); i++)
			{
				if (m_scene->lightList[i]->getType() == DIRECTIONAL_LIGHT)
				{
					graphics::DirectionalLight* light = (graphics::DirectionalLight*)m_scene->lightList[i];
					shader->setUniformFloat("u_directionalLight.base.ambientIntensity", (light->ambientIntensity));
					shader->setUniformFloat("u_directionalLight.base.diffuseIntensity", (light->diffuseIntensity));
					shader->setUniformVec3("u_directionalLight.base.color", glm::value_ptr(light->color));
					shader->setUniformVec3("u_directionalLight.direction", glm::value_ptr(light->getForward()));

					if (m_shadowStruct.find(light) != m_shadowStruct.end())
					{
						m_shadowStruct[light]->useTexture(0);
						shader->setUnifromMat4f("u_directionalLightTransform", glm::value_ptr(light->getLightTransform()));
					}
				}
			}

			for (graphics::Renderable* mesh : set.second)
			{
				if((mesh->getMaterialRef()->passes & COLOR_PASS) != 0)
					mesh->render(*m_renderer, shader);
			}
		}
		m_lineShader->bind();
		m_lineShader->setUnifromMat4f("u_projection", m_scene->mainCamera->getProjectionMatrix());
		m_lineShader->setUnifromMat4f("u_view", m_scene->mainCamera->getViewMatrix());
		for (auto& light : m_scene->lightList)
		{
			light->render(m_lineShader, m_renderer);
		}
	}

	void ColorPass::postRender() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ColorPass::setInputs(RenderPass* pass)
	{
		DirectionalShadowPass* input = (DirectionalShadowPass*)pass;
		input->getOutputs((void*)&m_shadowStruct);
	}

	void ColorPass::getOutputs(void* inputStruct)
	{
		std::vector<graphics::Texture*> *input = (std::vector<graphics::Texture*>*)inputStruct;
		input->push_back(m_colorTexture);
		input->push_back(m_depthTexture);
		for (size_t i = 0; i < m_scene->lightList.size(); i++)
		{
			if (m_scene->lightList[i]->getType() == DIRECTIONAL_LIGHT)
			{
				graphics::DirectionalLight* light = (graphics::DirectionalLight*)m_scene->lightList[i];
				if (m_shadowStruct.find(light) != m_shadowStruct.end())
				{
					input->push_back(m_shadowStruct[light]);
					input->push_back((graphics::Texture*)((void*)light));
				}
			}
		}
	}
}