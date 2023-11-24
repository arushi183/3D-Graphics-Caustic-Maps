#pragma once

#include <map>
#include "Shader.h"

namespace graphics
{
	class ShaderManager
	{
	public:
		static ShaderManager* getInstance();

		static Shader* loadShader(const char* vertexShader, const char* fragmentShader);

		static void terminate();
	private:
		static ShaderManager* m_instance;
		static std::map<std::pair<const char*, const char*>, Shader*> m_shaderLookup;
	};
}