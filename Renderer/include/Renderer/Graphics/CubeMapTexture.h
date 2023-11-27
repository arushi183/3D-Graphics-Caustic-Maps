#pragma once

namespace graphics
{

	class CubeMapTexture
	{
	public:
		CubeMapTexture(unsigned int width, unsigned int height, unsigned int bitDepth);

		~CubeMapTexture();

		void useTexture(unsigned int textureSlot);
		unsigned int getTextureID();
	private:
		unsigned int m_textureID;
		int m_width, m_height, m_bitDepth;

		const char* m_fileLocation;
	};

}