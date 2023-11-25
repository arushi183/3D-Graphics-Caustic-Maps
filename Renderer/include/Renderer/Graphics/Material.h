#pragma once

#include "ShaderManager.h"

#include <string>

namespace graphics
{

	struct Material
	{
	public:
		Material(Shader* shader = nullptr);
		
		void render();

		bool setParamInt(const char* uniform, int value);
		bool setParamFloat(const char* uniform, float value);
		bool setParamBool(const char* uniform, bool value);

		bool setParamVec2(const char* uniform, const float* value);
		bool setParamVec3(const char* uniform, const float* value);
		bool setParamVec4(const char* uniform, const float* value);

		Shader* shader;
	private:
		std::unordered_map<const char*, int> m_uniforms_int;
		std::unordered_map<const char*, float> m_uniforms_float;
		std::unordered_map<const char*, bool> m_uniforms_bool;
		std::unordered_map<const char*, const float*> m_uniforms_Vec2;
		std::unordered_map<const char*, const float*> m_uniforms_Vec3;
		std::unordered_map<const char*, const float*> m_uniforms_Vec4;
	};

}