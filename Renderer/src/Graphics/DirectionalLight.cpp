#include "Renderer/Graphics/DirectionalLight.h"
#include "Renderer/Graphics/Texture.h"

#include "Renderer/Renderer.h"

#include <glad/glad.h>

namespace graphics
{

	DirectionalLight::DirectionalLight(bool castShadows)
		:Light(), m_projMatrix(1.0f), m_viewMatrix(1.0f), m_shadowMap(nullptr), m_shadowFBO(0)
	{
		if (castShadows)
		{
			m_shadowMap = new Texture(SHADOW_WIDTH, SHADOW_HEIGHT, 1);
			m_projMatrix = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f);

			glGenFramebuffers(1, &m_shadowFBO);

			glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap->getTextureID(), 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		float points[] =
		{
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, &points[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	DirectionalLight::~DirectionalLight()
	{
		if (m_VBO != 0)
			glDeleteBuffers(1, &m_VBO);
		if (m_VAO != 0)
			glDeleteVertexArrays(1, &m_VAO);

		if (m_shadowFBO != 0)
		{
			delete(m_shadowMap);
			glDeleteFramebuffers(1, &m_shadowFBO);
		}
	}

	int DirectionalLight::getType()
	{
		return DIRECTIONAL_LIGHT;
	}

	const glm::mat4 DirectionalLight::getLightTransform()
	{
		glm::mat4 model(1.0f);

		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 forward = { 0.0f, 0.0f, 1.0f };

		forward = glm::vec3(glm::vec4(forward, 1.0f) * model);
		forward.z *= -1;
		forward *= -1;

		return m_projMatrix * glm::lookAt(forward * -10.0f, glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	const glm::vec3 DirectionalLight::getForward()
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 forward = { 0.0f, 0.0f, 1.0f };
		forward = glm::vec3(glm::vec4(forward, 1.0f) * model);
		forward.z *= -1;
		forward *= -1;

		return forward;
	}

	void DirectionalLight::render(Shader* shader, renderer::Renderer* render)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, transform.position);

		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));


		shader->setUnifromMat4f("u_model", glm::value_ptr(model));
		render->renderLine(m_VAO, 6);
	}

	void DirectionalLight::getShadowObject(unsigned int& shadowFBO, Texture*& shadowTexture)
	{
		shadowFBO = m_shadowFBO;
		shadowTexture = m_shadowMap;
	}
}