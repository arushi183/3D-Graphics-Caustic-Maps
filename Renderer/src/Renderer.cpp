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
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
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

	void Renderer::updateFrameBufferResolution(unsigned int width, unsigned int height)
	{
		m_framebuffer_width = width;
		m_framebuffer_height = height;

		glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);
	}

}