#pragma once
#include <unordered_map>

namespace graphics
{
	class Shader {
	public:
		Shader(const char* vertexShader, const char* fragmentShader);
		Shader(const char* vertexShader, const char* fragmentShader, const char* geometryShader);
		~Shader();

		void bind();
		void unbind();

		bool setUniformInt(const char* uniform, int value);
		bool setUniformFloat(const char* uniform, float value);
		bool setUniformBool(const char* uniform, bool value);

		bool setUniformVec2(const char* uniform, const float* value);
		bool setUniformVec3(const char* uniform, const float* value);
		bool setUniformVec4(const char* uniform, const float* value);

		bool setUnifromMat4f(const char* uniform, const float* value);

		const char *vertexShaderCode, *fragmentShaderCode, *geometryShaderCode;
	private:
		unsigned int m_program;
		std::unordered_map<const char*, unsigned int> m_uniformLookup;
	};
}