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
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		m_viewportWidth = m_viewport[2];
		m_viewportHeight = m_viewport[3];

		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_viewportWidth, m_viewportHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		m_colorTexture = new graphics::Texture(m_viewportWidth, m_viewportHeight, 3);
		glGenFramebuffers(1, &m_ColorFBO);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ColorFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture->getTextureID(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ColorPass::~ColorPass()
	{
		if (m_ColorFBO != 0)
		{
			delete(m_colorTexture);
			glDeleteFramebuffers(1, &m_ColorFBO);
			glDeleteRenderbuffers(1, &m_RBO);
		}
	}

	void ColorPass::preRender()
	{
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
			// and inputs

			for (graphics::Renderable* mesh : set.second)
				mesh->render(*m_renderer);
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
		graphics::Texture*** input = (graphics::Texture***)inputStruct;
		*(input) = &m_colorTexture;
	}
}