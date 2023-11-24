#include "Renderer/Graphics/Mesh.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>

namespace graphics
{
	Mesh::Mesh(float* vertices, const unsigned int numVertices, unsigned int* indices, const unsigned int numIndices, unsigned int* vertexLayout, const unsigned int numVertexLayout)
	{
		m_indexCount = numIndices;

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_IBO);
		
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);

		unsigned int vertexPacketSize = 0;
		unsigned int vertexAttributeStride = 0;
		for (int i = 0; i < numVertexLayout; i++)
			vertexPacketSize += vertexLayout[i];

		for (unsigned int i = 0; i < numVertexLayout; i++) {
			glVertexAttribPointer(i, vertexLayout[i], GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * vertexPacketSize, (void*)vertexAttributeStride);
			glEnableVertexAttribArray(i);

			vertexAttributeStride += sizeof(vertices[0]) * vertexLayout[i];
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		if (m_IBO != 0)
			glDeleteBuffers(1, &m_IBO);
		if (m_VBO != 0)
			glDeleteBuffers(1, &m_VBO);
		if (m_VAO != 0)
			glDeleteVertexArrays(1, &m_VAO);
	}

	void Mesh::render(renderer::Renderer* renderer)
	{
		renderer->render(m_VAO, m_indexCount);
	}
}