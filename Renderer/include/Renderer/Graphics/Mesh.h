#pragma once

namespace renderer
{
	class Renderer;
}

namespace graphics
{

	class Mesh
	{
	public:
		Mesh(float* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, unsigned int* vertexLayout, unsigned int numVertexLayout);
		~Mesh();

		void render(renderer::Renderer* renderer);
	private:
		unsigned int m_VAO, m_VBO, m_IBO;
		int m_indexCount;
	};

}