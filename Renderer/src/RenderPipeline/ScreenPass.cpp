#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/ScreenPass.h"
#include "Renderer/RenderPipeline/ColorPass.h"
#include "Renderer/Graphics/ShaderManager.h"
#include "Renderer/Graphics/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace renderer
{

	ScreenPass::ScreenPass(Renderer* renderer, graphics::Shader* screenShader)
		:RenderPass(renderer), m_screenShader(screenShader)
	{
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		m_viewportWidth = m_viewport[2];
		m_viewportHeight = m_viewport[3];

		m_colorTexture = nullptr;

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
		m_colorTexture->useTexture(0);
		m_renderer->render(m_VAO, 6);
	}

	void ScreenPass::postRender()
	{
	}

	void ScreenPass::setInputs(RenderPass* pass)
	{
		ColorPass* input = (ColorPass*)pass;
		graphics::Texture** colorTextureRef;
		input->getOutputs((void*)&colorTextureRef);
		m_colorTexture = *colorTextureRef;
	}

	void ScreenPass::getOutputs(void* inputStruct)
	{
	}
}