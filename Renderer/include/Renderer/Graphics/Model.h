#pragma once

#include "Renderable.h"

#include "Mesh.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include <vector>

struct aiNode;
struct aiMesh;
struct aiScene;

namespace graphics
{

	class Model: public Renderable
	{
	public:
		Model(const char* fileLoc, Shader* overrideShader = nullptr);
		Model(float* vertices, unsigned int numVerts, unsigned int* indices, unsigned int numInd, unsigned int* vertexAttributes, unsigned int numVertAttribs, Shader* overrideShader = nullptr);
		~Model();

		void render(renderer::Renderer& render) override;
		int type() override;
		Shader* getShaderRef() const override;
		
	private:
		std::vector<Mesh*> m_meshList;
		std::vector<Texture*> m_textureList;
		std::vector<unsigned int> m_meshToTex;

		Shader* m_shader;

		void LoadNode(aiNode* node, const aiScene* scene);
		void LoadMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterials(const aiScene* scene);
	};

}