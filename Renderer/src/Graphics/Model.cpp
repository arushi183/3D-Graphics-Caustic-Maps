#include "Renderer/Graphics/Model.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

namespace graphics
{
	Model::Model(const char* fileLoc, Material* overrideMaterial)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(fileLoc, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
		
		if (!scene)
		{
			//Model could not be loaded
			return;
		}

		if (overrideMaterial != nullptr)
			m_material = overrideMaterial;
		else
			m_material = new Material();

		LoadNode(scene->mRootNode, scene);
		LoadMaterials(scene);
	}

	Model::Model(float* vertices, unsigned int numVerts, unsigned int* indices, unsigned int numInd, unsigned int* vertexAttributes, unsigned int numVertAttribs, Material* overrideMaterial)
	{
		m_meshList.push_back(new Mesh(vertices, numVerts, indices, numInd, vertexAttributes, numVertAttribs));
		m_meshToTex.push_back(0);
		if (overrideMaterial != nullptr)
			m_material = overrideMaterial;
		else
			m_material = new Material();
	}

	Model::~Model()
	{
		for (size_t i = 0; i < m_meshList.size(); i++)
		{
			if (m_meshList[i])
				delete(m_meshList[i]);
		}
	}

	void Model::render(renderer::Renderer& render, Shader* overrideShader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.position);

		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, transform.scale);

		if(overrideShader  != nullptr)
			overrideShader->setUnifromMat4f("u_model", glm::value_ptr(model));
		else
		{
			m_material->shader->setUnifromMat4f("u_model", glm::value_ptr(model));
			m_material->render();
		}

		for (size_t i = 0; i < m_meshList.size(); i++)
		{
			unsigned int materialIndex = m_meshToTex[i];
			if (materialIndex < m_textureList.size() && m_textureList[materialIndex]) {
				m_textureList[materialIndex]->useTexture(1);
			}
			m_meshList[i]->render(&render);
		}
	}

	int Model::type()
	{
		return GRAPHICS_MODEL;
	}

	Shader* Model::getShaderRef() const
	{
		return m_material->shader;
	}

	void Model::LoadNode(aiNode* node, const aiScene* scene)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
			LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);

		for (size_t i = 0; i < node->mNumChildren; i++)
			LoadNode(node->mChildren[i], scene);
	}

	void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
			if (mesh->mTextureCoords[0]) {
				vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
			}
			else {
				vertices.insert(vertices.end(), { 0.0f, 0.0f });
			}
			vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		std::vector<unsigned int> vertexAttribLayout = { 3,2,3 };

		m_meshList.push_back(new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), &vertexAttribLayout[0], vertexAttribLayout.size()));
		m_meshToTex.push_back(mesh->mMaterialIndex);
	}

	void Model::LoadMaterials(const aiScene* scene)
	{
		m_textureList.resize(scene->mNumMaterials);
		for (size_t i = 0; i < scene->mNumMaterials; i++) {
			aiMaterial* material = scene->mMaterials[i];

			m_textureList[i] = nullptr;

			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString path;
				aiString texture_file;
				material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);
				if (auto texture = scene->GetEmbeddedTexture(texture_file.C_Str()))
				{
					if (texture->mHeight == 0)
					{
						char* compressed_texture_data = reinterpret_cast<char*> (texture->pcData);
						m_textureList[i] = new Texture(compressed_texture_data, texture->mWidth * 4);
					}
					else
					{
						aiTexel* ARGB = texture->pcData;
						const size_t numChannels = 4;
						const size_t dataSize = texture->mWidth * texture->mHeight * numChannels;

						unsigned char* textureData = new unsigned char[dataSize];

						int idx = 0;
						for (size_t i = 0; i < dataSize; i += 4) {
							textureData[i + 0] = ARGB[idx].r;
							textureData[i + 1] = ARGB[idx].g;
							textureData[i + 2] = ARGB[idx].b;
							textureData[i + 3] = ARGB[idx].a;

							idx++;
						}

						m_textureList[i] = new Texture(texture->mWidth, texture->mHeight, textureData);

						delete[] textureData;
					}
				}
				else if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					int idx = std::string(path.data).rfind("\\");
					std::string filename = std::string(path.data).substr(idx + 1);

					std::string texPath = std::string("res/textures/") + filename;

					m_textureList[i] = TextureManager::getInstance()->loadTexture(texPath.c_str());

					if (!m_textureList[i]) {
						printf("Failed to load texture at: %s\n", texPath);
						delete m_textureList[i];
						m_textureList[i] = nullptr;
					}
				}
			}
			if (!m_textureList[i]) {
				m_textureList[i] = TextureManager::getInstance()->loadTexture("res/textures/plain.png");
			}
		}
	}
}