#include "Renderer/Renderer.h"

#include "Renderer/RenderPipeline/CausticMapPass.h"
#include "Renderer/Graphics/ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Graphics/DirectionalLight.h"

namespace renderer
{

	CausticMapPass::CausticMapPass(graphics::Scene* scene, Renderer* renderer, graphics::Material* material)
		:RenderPass(renderer), m_scene(scene), m_causticMapShader(nullptr)
	{
		m_causticMaterial = material;
		m_causticMapShader = m_causticMaterial->shader;

		int resolution = 500;

		std::vector<float> verts;
		std::vector<unsigned int> ind;
		std::vector<unsigned int> attribs = { 3 };

		for (int y = 0; y < resolution; y++)
		{
			for (int x = 0; x < resolution; x++)
			{
				float tx = ((float)x / (float)(resolution - 1))*2-1;
				float ty = ((float)y / (float)(resolution - 1))*2-1;
				verts.insert(verts.end(), { -0.5f + tx, 0, -0.5f + ty });
			}
		}

		for (int y = 0; y < resolution - 1; y++)
		{
			for (int x = 0; x < resolution - 1; x++)
			{
				unsigned int quad = y * resolution + x;
				ind.insert(ind.end(), { quad, quad + resolution, quad + resolution + 1 });
				ind.insert(ind.end(), { quad, quad + resolution + 1, quad + 1 });
			}
		}

		m_quadMap = new graphics::Mesh(verts.data(), verts.size(), ind.data(), ind.size(), attribs.data(), attribs.size());

		//m_causticMaterial->setParamVec3("oldPosition", temp);
		m_projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f,0.1f,100.0f);

		m_causticTexture = new graphics::Texture(720, 720, 3);
		glGenFramebuffers(1, &m_causticFBO);
		glGenRenderbuffers(1, &m_causticRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_causticRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 720, 720);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, m_causticFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_causticTexture->getTextureID(), 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_causticRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}

	void CausticMapPass::preRender()
	{
		glViewport(0, 0, 720, 720);

		glBindFramebuffer(GL_FRAMEBUFFER, m_causticFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_causticMapShader->bind();
	}

	void CausticMapPass::render()
	{
		//glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
		const float temp[3] = { 0,-1,0 };
		m_causticMapShader->setUniformVec3("light", &temp[0]);
		m_quadMap->render(m_renderer);
	}

	void CausticMapPass::postRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void CausticMapPass::setInputs(RenderPass* pass)
	{

	}

	void CausticMapPass::getOutputs(void* inputStruct)
	{
		std::vector<graphics::Texture*>* colorTextureRef = (std::vector<graphics::Texture*>*) inputStruct;
		colorTextureRef->push_back(m_causticTexture);
	}

}