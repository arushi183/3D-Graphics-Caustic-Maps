#include "Renderer/Graphics/CubeMapTexture.h"

#include <glad/glad.h>
#include <iostream>
namespace graphics
{

	CubeMapTexture::CubeMapTexture(unsigned int width, unsigned int height, unsigned int bitDepth)
		:m_textureID(0), m_height(height), m_width(width), m_bitDepth(bitDepth), m_fileLocation(nullptr)
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

		if(bitDepth == 1)
			for (size_t i = 0; i < 6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		else if(bitDepth == 3)
			for (size_t i = 0; i < 6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	CubeMapTexture::~CubeMapTexture()
	{
		if (m_textureID)
			glDeleteTextures(1, &m_textureID);
	}

	void CubeMapTexture::useTexture(unsigned int textureSlot)
	{
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	}

	unsigned int CubeMapTexture::getTextureID()
	{
		return m_textureID;
	}
}