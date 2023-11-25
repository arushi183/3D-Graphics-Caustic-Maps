#include "Renderer/Graphics/Material.h"

namespace graphics
{

	Material::Material(Shader* shader, bool reciveShadows)
	{
		this->reciveShadows = reciveShadows;
		if (shader == nullptr)
			this->shader = ShaderManager::getInstance()->loadShader("res/shaders/default.vert", "res/shaders/default.frag");
		else
			this->shader = shader;
	}

	void Material::render()
	{
		for (auto& e : m_uniforms_int)
			shader->setUniformInt(e.first, e.second);
		for (auto& e : m_uniforms_float)
			shader->setUniformFloat(e.first, e.second);
		for (auto& e : m_uniforms_bool)
			shader->setUniformBool(e.first, e.second);
		for (auto& e : m_uniforms_Vec2)
			shader->setUniformVec2(e.first, e.second);
		for (auto& e : m_uniforms_Vec3)
			shader->setUniformVec3(e.first, e.second);
		for (auto& e : m_uniforms_Vec4)
			shader->setUniformVec4(e.first, e.second);
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

}