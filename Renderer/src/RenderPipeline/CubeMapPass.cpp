#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/CubeMapPass.h"
#include "Renderer/Graphics/ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Graphics/DirectionalLight.h"
namespace renderer
{
	CubeMapPass::CubeMapPass(graphics::Scene* scene, Renderer* renderer)
		:RenderPass(renderer), m_scene(scene)
	{
		m_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
	}

	CubeMapPass::~CubeMapPass()
	{}

	void CubeMapPass::preRender()
	{
		m_DrawList.clear();
		glViewport(0, 0, 720, 720);
		for (auto& set : m_scene->meshList)
		{
			for (auto& mesh : set.second)
			{
				if ((mesh->getMaterialRef()->passes & CUBEMAP_PASS) != 0)
				{
					m_DrawList.push_back(mesh);
					glBindFramebuffer(GL_FRAMEBUFFER, mesh->getMaterialRef()->CubeMapFBO);
					for (int i = 0; i < 6; i++) {
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mesh->getMaterialRef()->cubeMapTexture->getTextureID(), 0);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					}
				}
			}
		}
	}

	void CubeMapPass::render()
	{
		for (auto& target : m_DrawList)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, target->getMaterialRef()->CubeMapFBO);

			std::vector<glm::mat4> cubeMatrices;
			cubeMatrices.push_back(glm::lookAt(target->transform.position, target->transform.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			cubeMatrices.push_back(glm::lookAt(target->transform.position, target->transform.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			cubeMatrices.push_back(glm::lookAt(target->transform.position, target->transform.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			cubeMatrices.push_back(glm::lookAt(target->transform.position, target->transform.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
			cubeMatrices.push_back(glm::lookAt(target->transform.position, target->transform.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
			cubeMatrices.push_back(glm::lookAt(target->transform.position, target->transform.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

			for (int i = 0; i < cubeMatrices.size(); i++)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, target->getMaterialRef()->cubeMapTexture->getTextureID(), 0);
				
				for (auto& set : m_scene->meshList)
				{
					graphics::Shader* shader = set.first;
					shader->bind();

					shader->setUnifromMat4f("u_projection", glm::value_ptr(m_projection));
					shader->setUnifromMat4f("u_view", glm::value_ptr(cubeMatrices[i]));
					shader->setUniformVec3("u_cameraPos", glm::value_ptr(target->transform.position));
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
						if (mesh != target && (mesh->getMaterialRef()->passes & COLOR_PASS) != 0)
							mesh->render(*m_renderer, shader);
					}
				}
				
				glClear(GL_DEPTH_BUFFER_BIT);
			}
		}
	}

	void CubeMapPass::postRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void CubeMapPass::setInputs(RenderPass* pass)
	{
		pass->getOutputs(nullptr);
	}

	void CubeMapPass::getOutputs(void* inputStruct)
	{

	}

}