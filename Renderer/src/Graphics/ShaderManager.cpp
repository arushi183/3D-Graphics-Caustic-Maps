#include "Renderer/Graphics/ShaderManager.h"

namespace graphics
{
	ShaderManager* ShaderManager::m_instance = nullptr;
	std::map<std::pair<const char*, const char*>, Shader*> ShaderManager::m_shaderLookup;

	ShaderManager* ShaderManager::getInstance()
	{
		if (m_instance != nullptr)
			return m_instance;
		else
			m_instance = new ShaderManager();
		return m_instance;
	}
	Shader* ShaderManager::loadShader(const char* vertexShader, const char* fragmentShader)
	{
		if (m_shaderLookup.find({ vertexShader, fragmentShader }) != m_shaderLookup.end())
			return m_shaderLookup[{vertexShader, fragmentShader}];
		else
			return m_shaderLookup[{vertexShader, fragmentShader}] = new Shader(vertexShader, fragmentShader);
	}

	void ShaderManager::terminate()
	{
		delete(m_instance);
		for (auto& entry : m_shaderLookup)
			delete(entry.second);
	}
}