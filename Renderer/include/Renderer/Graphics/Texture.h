#pragma once

namespace graphics
{

	class Texture
	{
	public:
		Texture(const char* fileLoc);
		Texture(const char* texData, unsigned int texSize);
		Texture(unsigned int width, unsigned int height, unsigned int bitDepth);
		Texture(unsigned int width, unsigned int height, unsigned char* textureData, unsigned int bitDepth = 4);
		
		~Texture();

		void useTexture(unsigned int textureSlot);
		unsigned int getTextureID();
	private:
		unsigned int m_textureID;
		int m_width, m_height, m_bitDepth;

		const char* m_fileLocation;
	};

}