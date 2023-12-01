#include "Renderer/Renderer.h"
#include "Renderer/Graphics/Mesh.h"

#include "Renderer/RenderPipeline/CausticMapPass.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace renderer
{

	CausticMapPass::CausticMapPass(graphics::Scene* scene, Renderer* renderer, graphics::Material* causticGenMaterial)
		:RenderPass(renderer), m_scene(scene), m_causticFBO(0), m_environmentFBO(0), m_causticRBO(0), m_CausticTexture(nullptr), m_EnvdepthTexture(nullptr), m_depthShader(nullptr), m_lightWavefront(nullptr), m_causticGenMaterial(causticGenMaterial)
	{
		m_projection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 0.1f, 100.0f);
		m_view = glm::lookAt(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0) + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));

		m_depthShader = graphics::ShaderManager::getInstance()->loadShader("res/shaders/depth.vert", "res/shaders/depth.frag");

		m_EnvdepthTexture = new graphics::Texture(1024, 1024, 1);
		m_CausticTexture = new graphics::Texture(1024, 1024, 3);

		glGenFramebuffers(1, &m_environmentFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_environmentFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_EnvdepthTexture->getTextureID(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenFramebuffers(1, &m_causticFBO);
		glGenRenderbuffers(1, &m_causticRBO);

		glBindRenderbuffer(GL_RENDERBUFFER, m_causticRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_causticFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_CausticTexture->getTextureID(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_causticRBO);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int resolution = 500;

		std::vector<float> verts;
		std::vector<unsigned int> ind;
		std::vector<unsigned int> attribs = { 3, 2 };

		for (int y = 0; y < resolution; y++)
		{
			float ty = (float)y / (float)(resolution - 1);
			for (int x = 0; x < resolution; x++)
			{
				float tx = (float)x / (float)(resolution - 1);
				verts.insert(verts.end(), { (- 0.5f + tx) * 2, (-0.5f + ty) * 2, 0, tx, ty});
			}
		}
		for (int y = 0; y < resolution - 1; y++)
		{
			for (int x = 0; x < resolution - 1; x++)
			{
				unsigned int quad = y * resolution + x;
				ind.insert(ind.end(), { quad + resolution + 1, quad + resolution, quad });
				ind.insert(ind.end(), { quad + 1, quad + resolution + 1, quad });
			}
		}

		m_lightWavefront = new graphics::Mesh(verts.data(), verts.size(), ind.data(), ind.size(), attribs.data(), attribs.size());
	}

	CausticMapPass::~CausticMapPass()
	{
		if (m_environmentFBO != 0)
			glDeleteFramebuffers(1, &m_environmentFBO);
		if (m_causticFBO != 0)
			glDeleteFramebuffers(1, &m_causticFBO);
		if (m_causticRBO != 0)
			glDeleteRenderbuffers(1, &m_causticRBO);
		
		delete(m_EnvdepthTexture);
		delete(m_CausticTexture);
		delete(m_lightWavefront);
	}

	void CausticMapPass::preRender()
	{
		glViewport(0, 0, 1024, 1024);

		glBindFramebuffer(GL_FRAMEBUFFER, m_causticFBO);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, m_environmentFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void CausticMapPass::render()
	{
		m_depthShader->bind();
		m_depthShader->setUniformInt("t_alphaTex", 1);
		m_depthShader->setUnifromMat4f("u_projection_view", glm::value_ptr(m_projection * m_view));
		for (auto& set : m_scene->meshList)
		{
			for (graphics::Renderable* mesh : set.second)
			{
				if ((mesh->getMaterialRef()->passes & CAUSTIC_PASS) != 0)
					mesh->render(*m_renderer, m_depthShader);
			}
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, m_causticFBO);

		// Shader uniforms
		float viewPos[] = { 0,1,0 };
		m_causticGenMaterial->shader->bind();
		m_causticGenMaterial->shader->setUnifromMat4f("u_projection", glm::value_ptr(m_projection));
		m_causticGenMaterial->shader->setUnifromMat4f("u_view", glm::value_ptr(m_view));
		m_causticGenMaterial->shader->setUniformVec3("u_cameraPos", &viewPos[0]);
		m_causticGenMaterial->shader->setUniformFloat("m_near", 0.1f);
		m_causticGenMaterial->shader->setUniformFloat("m_far", 100.0f);
		m_causticGenMaterial->shader->setUniformInt("t_environmentDepth", 0);
		m_EnvdepthTexture->useTexture(0);
		m_causticGenMaterial->render(m_causticGenMaterial->shader);
		
		//Render Caustics Map
		m_lightWavefront->render(m_renderer);
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
		for (auto& set : m_scene->meshList)
		{
			for (graphics::Renderable* mesh : set.second)
			{
				if ((mesh->getMaterialRef()->passes & CAUSTIC_PASS) != 0) {
					mesh->getMaterialRef()->setCausticTexture(m_CausticTexture, glm::value_ptr(m_projection), glm::value_ptr(m_view));
				}
			}
		}
	}
}