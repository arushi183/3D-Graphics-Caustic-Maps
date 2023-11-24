#pragma once

#include <map>
#include "Texture.h"

namespace graphics
{
	class TextureManager
	{
	public:
		static TextureManager* getInstance();

		static Texture* loadTexture(const char* textureFile);

		static void terminate();
	private:
		static TextureManager* m_instance;
		static std::map<const char*, Texture*> m_textureLookup;
	};
}