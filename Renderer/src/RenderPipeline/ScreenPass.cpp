#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/ScreenPass.h"
#include "Renderer/RenderPipeline/ColorPass.h"
#include "Renderer/Graphics/ShaderManager.h"
#include "Renderer/Graphics/Texture.h"
#include "Renderer/Graphics/DirectionalLight.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace renderer
{

	ScreenPass::ScreenPass(graphics::Scene* scene, Renderer* renderer, graphics::Shader* screenShader)
		:RenderPass(renderer), m_screenShader(screenShader), m_scene(scene)
	{
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		m_viewportWidth = m_viewport[2];
		m_viewportHeight = m_viewport[3];

		m_colorTexture = nullptr;
		m_depthTexture = nullptr;
		m_shadowTexture = nullptr;

		float vertices[] = {
			-1, -1, 0, 0, 0,
			 1, -1, 0, 1, 0,
			-1,  1, 0, 0, 1,
			 1,  1, 0, 1, 1
		};

		unsigned int indices[] = {
			0,1,3,
			0,3,2
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_IBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * 20, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * 6, indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
		glEnableVertexAttribArray(1);
	}

	ScreenPass::~ScreenPass()
	{
		if (m_IBO != 0)
			glDeleteBuffers(1, &m_IBO);
		if (m_VBO != 0)
			glDeleteBuffers(1, &m_VBO);
		if (m_VAO != 0)
			glDeleteVertexArrays(1, &m_VAO);
	}

	void ScreenPass::preRender()
	{
		glViewport(0, 0, m_viewportWidth, m_viewportHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ScreenPass::render()
	{
		m_screenShader->bind();

		m_screenShader->setUnifromMat4f("u_view", m_scene->mainCamera->getViewMatrix());
		m_screenShader->setUnifromMat4f("u_projection", m_scene->mainCamera->getProjectionMatrix());
		m_screenShader->setUnifromMat4f("u_directionalLightTransform", glm::value_ptr(m_lightTransform));

		m_screenShader->setUniformVec3("u_cameraPos", glm::value_ptr(m_scene->mainCamera->transform.position));

		m_screenShader->setUniformFloat("m_near", 0.1f);
		m_screenShader->setUniformFloat("m_far", 1000.0f);

		m_screenShader->setUniformInt("t_mainTex", 0);
		m_screenShader->setUniformInt("t_depthTex", 1);
		m_screenShader->setUniformInt("t_shadowMapTex", 2);

		m_colorTexture->useTexture(0);
		m_depthTexture->useTexture(1);
		m_shadowTexture->useTexture(2);

		m_renderer->render(m_VAO, 6);
	}

	void ScreenPass::postRender()
	{
	}

	void ScreenPass::setInputs(RenderPass* pass)
	{
		ColorPass* input = (ColorPass*)pass;
		std::vector<graphics::Texture*> colorTextureRef;
		input->getOutputs((void*)&colorTextureRef);
		m_colorTexture = colorTextureRef[0];
		m_depthTexture = colorTextureRef[1];
		m_shadowTexture = colorTextureRef[2];
		graphics::DirectionalLight* light = ((graphics::DirectionalLight*)((void*)colorTextureRef[3]));
		m_lightTransform = ((graphics::DirectionalLight*)((void*)colorTextureRef[3]))->getLightTransform();
	}

	void ScreenPass::getOutputs(void* inputStruct)
	{
	}
}