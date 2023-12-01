#pragma once

#include "ShaderManager.h"
#include "CubeMapTexture.h"
#include "Texture.h"

#include <string>

namespace graphics
{

#define SHADOW_PASS 1
#define COLOR_PASS 2
#define CUBEMAP_PASS 4
#define CAUSTIC_PASS 8

	struct Material
	{
	public:
		Material(Shader* shader = nullptr, unsigned int pass = 3);
		~Material();

		void render(Shader* overrideShader);

		void setCausticTexture(Texture* caustic, float* causticProj, float* causticView);

		bool setParamInt(const char* uniform, int value);
		bool setParamFloat(const char* uniform, float value);
		bool setParamBool(const char* uniform, bool value);

		bool setParamVec2(const char* uniform, const float* value);
		bool setParamVec3(const char* uniform, const float* value);
		bool setParamVec4(const char* uniform, const float* value);

		bool setParamMat4(const char* uniform, const float* value);

		Shader* shader;
		uint32_t passes;
		unsigned int CubeMapFBO;
		CubeMapTexture* cubeMapTexture;

		Texture* causticMap;
		float *causticProj, *causticView;
	private:
		std::unordered_map<const char*, int> m_uniforms_int;
		std::unordered_map<const char*, float> m_uniforms_float;
		std::unordered_map<const char*, bool> m_uniforms_bool;
		std::unordered_map<const char*, const float*> m_uniforms_Vec2;
		std::unordered_map<const char*, const float*> m_uniforms_Vec3;
		std::unordered_map<const char*, const float*> m_uniforms_Vec4;
		std::unordered_map<const char*, const float*> m_uniforms_Mat4;

		unsigned int m_RBO;
	};

}