#include "Renderer/Graphics/TextureManager.h"

namespace graphics
{
	TextureManager* TextureManager::m_instance = nullptr;
	std::map<const char*, Texture*> TextureManager::m_textureLookup;

	TextureManager* TextureManager::getInstance()
	{
		if (m_instance != nullptr)
			return m_instance;
		else
			m_instance = new TextureManager();
		return m_instance;
	}
	Texture* TextureManager::loadTexture(const char* textureLocation)
	{
		if (m_textureLookup.find(textureLocation) != m_textureLookup.end())
			return m_textureLookup[textureLocation];
		else
			return m_textureLookup[textureLocation] = new Texture(textureLocation);
	}

	void TextureManager::terminate()
	{
		delete(m_instance);
		for (auto& entry : m_textureLookup)
			delete(entry.second);
	}
}