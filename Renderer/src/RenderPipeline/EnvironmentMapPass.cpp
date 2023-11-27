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
		m_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
	}

	void EnvironmentMapPass::preRender()
	{
		m_environmentMapShader->bind();

		
		glViewport(0, 0, 720, 720);
		glBindFramebuffer(GL_FRAMEBUFFER, 
		/*for (auto& set : m_scene->meshList)
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
		}*/
		
	}

	void EnvironmentMapPass::render()
	{
		
		for (auto& target : m_DrawList)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, target->getMaterialRef()->CubeMapFBO);

			std::vector<glm::mat4> cubeMatrices;
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
					for (graphics::Renderable* mesh : set.second)
					{
						if (mesh != target && (mesh->getMaterialRef()->passes & COLOR_PASS) != 0)
							mesh->render(*m_renderer, shader);
					}
				}
			}
		}
	}

	void EnvironmentMapPass::postRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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