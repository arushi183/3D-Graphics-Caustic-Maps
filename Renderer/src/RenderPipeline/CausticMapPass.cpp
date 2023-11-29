#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/CausticMapPass.h"
#include "Renderer/Graphics/ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Graphics/DirectionalLight.h"

namespace renderer {
	CausticMapPass::CausticMapPass(graphics::Scene* scene, Renderer* renderer)
		:RenderPass(renderer), m_scene(scene), m_causticMapShader(nullptr)
	{
		m_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

		m_causticTexture = new graphics::Texture(720, 720, 3);
		m_depthTexture = new graphics::Texture(720, 720, 1);
		glGenFramebuffers(1, &m_causticFBO);

		glBindFramebuffer(GL_FRAMEBUFFER, m_causticFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_causticTexture->getTextureID(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->getTextureID(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void CausticMapPass::preRender()
	{
		//m_environmentMapShader->bind();
		glViewport(0, 0, 720, 720);

		glBindFramebuffer(GL_FRAMEBUFFER, m_causticFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}	

	void CausticMapPass::render()
	{
		glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
		for (auto& set : m_scene->meshList)
		{
			graphics::Shader* shader = set.first;
			shader->bind();

			shader->setUnifromMat4f("u_projection", glm::value_ptr(m_projection));
			shader->setUnifromMat4f("u_view", glm::value_ptr(viewMatrix));

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

				}
			}
			for (graphics::Renderable* mesh : set.second)
			{
				
			}
		}
	}

	void CausticMapPass::postRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void CausticMapPass::setInputs(RenderPass* pass)
	{
	}

	void CausticMapPass::getOutputs(void* inputStruct)
	{
		std::vector<graphics::Texture*>* colorTextureRef = (std::vector<graphics::Texture*>*) inputStruct;
		colorTextureRef->push_back(m_causticTexture);
		colorTextureRef->push_back(m_depthTexture);
	}
}