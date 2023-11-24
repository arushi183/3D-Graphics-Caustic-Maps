#pragma once

namespace graphics
{

	class Texture
	{
	public:
		Texture(const char* fileLoc);
		~Texture();

		void useTexture(unsigned int textureSlot);
	private:
		unsigned int m_textureID;
		int m_width, m_height, m_bitDepth;

		const char* m_fileLocation;
	};

}