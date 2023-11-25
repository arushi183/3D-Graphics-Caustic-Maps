#include "Renderer/Renderer.h"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace renderer
{

	Renderer::Renderer(window::Window* window)
		:m_framebuffer_height(0), m_framebuffer_width(0)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "OpenGL: Failed to initialize";
			return;
		}

		FuncPtr frameupdateCallback = [this](unsigned int a, unsigned int b) { updateFrameBufferResolution(a, b); };
		window->addSizeCallback(frameupdateCallback);
		
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glEnable(GL_LINE_WIDTH);
		glLineWidth(2);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::render(unsigned int VAO, int elementCount)
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Renderer::renderLine(unsigned int VAO, int elementCount)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, elementCount);
		glBindVertexArray(0);
	}

	void Renderer::updateFrameBufferResolution(unsigned int width, unsigned int height)
	{
		m_framebuffer_width = width;
		m_framebuffer_height = height;

		glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);
	}

}