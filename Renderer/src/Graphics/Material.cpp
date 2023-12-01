#include "Renderer/Graphics/Material.h"

#include <glad/glad.h>

namespace graphics
{

	Material::Material(Shader* shader, unsigned int pass)
		:cubeMapTexture(nullptr), CubeMapFBO(0), m_RBO(0), causticMap(nullptr)
	{
		this->passes = pass;
		if (shader == nullptr)
			this->shader = ShaderManager::getInstance()->loadShader("res/shaders/default.vert", "res/shaders/default.frag");
		else
			this->shader = shader;

		if ((pass & CUBEMAP_PASS) != 0)
		{
			glGenRenderbuffers(1, &m_RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 720, 720);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			cubeMapTexture = new CubeMapTexture(720, 720, 3);
			glGenFramebuffers(1, &CubeMapFBO);

			glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	Material::~Material()
	{
		if (cubeMapTexture != nullptr)
		{
			delete(cubeMapTexture);
			glDeleteFramebuffers(1, &CubeMapFBO);
			glDeleteRenderbuffers(1, &m_RBO);
		}
	}

	void Material::render(Shader* overrideShader)
	{
		for (auto& e : m_uniforms_int)
			overrideShader->setUniformInt(e.first, e.second);
		for (auto& e : m_uniforms_float)
			overrideShader->setUniformFloat(e.first, e.second);
		for (auto& e : m_uniforms_bool)
			overrideShader->setUniformBool(e.first, e.second);
		for (auto& e : m_uniforms_Vec2)
			overrideShader->setUniformVec2(e.first, e.second);
		for (auto& e : m_uniforms_Vec3)
			overrideShader->setUniformVec3(e.first, e.second);
		for (auto& e : m_uniforms_Vec4)
			overrideShader->setUniformVec4(e.first, e.second);
		for (auto& e : m_uniforms_Mat4)
			overrideShader->setUnifromMat4f(e.first, e.second);
		
		if (cubeMapTexture != nullptr)
		{
			overrideShader->setUniformInt("t_cubeMap", 3);
			cubeMapTexture->useTexture(3);
		}
		else if (causticMap != nullptr)
		{
			overrideShader->setUniformInt("t_causticTexture", 3);
			causticMap->useTexture(3);
		}
	}

	void Material::setCausticTexture(Texture* caustic, float* causticProj, float* causticView)
	{
		causticMap = caustic;
		this->causticView = causticView;
		this->causticProj = causticProj;
		setParamMat4("u_causticView", causticView);
		setParamMat4("u_causticProj", causticProj);
	}

	bool Material::setParamInt(const char* uniform, int value)
	{
		m_uniforms_int[uniform] = value;
		return true;
	}

	bool Material::setParamFloat(const char* uniform, float value)
	{
		m_uniforms_float[uniform] = value;
		return true;
	}

	bool Material::setParamBool(const char* uniform, bool value)
	{
		m_uniforms_bool[uniform] = value;
		return true;
	}

	bool Material::setParamVec2(const char* uniform, const float* value)
	{
		m_uniforms_Vec2[uniform] = value;
		return true;
	}

	bool Material::setParamVec3(const char* uniform, const float* value)
	{
		m_uniforms_Vec3[uniform] = value;
		return true;
	}

	bool Material::setParamVec4(const char* uniform, const float* value)
	{
		m_uniforms_Vec4[uniform] = value;
		return true;
	}

	bool Material::setParamMat4(const char* uniform, const float* value)
	{
		m_uniforms_Mat4[uniform] = value;
		return true;
	}

}