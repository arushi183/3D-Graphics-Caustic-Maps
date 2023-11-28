#include "Renderer/Graphics/Texture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION 
#include "../../stb_image.h"

namespace graphics
{

	Texture::Texture(const char* fileLoc)
		:m_textureID(0), m_width(0), m_height(0), m_bitDepth(0), m_fileLocation(fileLoc)
	{
		unsigned char* texData;
		texData = stbi_load(m_fileLocation, &m_width, &m_height, &m_bitDepth, 0);
		if (!texData)
		{
			delete(this);
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(m_bitDepth == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		else if(m_bitDepth == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		else
		{
			//Invalid texture format
			stbi_image_free(texData);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &m_textureID);
			delete(this);
			return;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(texData);
		return;
	}

	Texture::Texture(unsigned int width, unsigned int height, unsigned int bitDepth)
		:m_textureID(0), m_width(width), m_height(height), m_bitDepth(bitDepth), m_fileLocation(nullptr)
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		if(m_bitDepth == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		if (m_bitDepth == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		float bColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);

		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	Texture::Texture(unsigned int width, unsigned int height, unsigned char* textureData, unsigned int bitDepth)
		:m_textureID(0), m_width(width), m_height(height), m_bitDepth(bitDepth), m_fileLocation(nullptr)
	{
		m_width = width;
		m_height = height;
		m_bitDepth = bitDepth;
		if (!textureData)
		{
			delete(this);
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_bitDepth == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		else if (m_bitDepth == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &m_textureID);
			delete(this);
			return;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return;
	}

	Texture::Texture(const char* texureData, unsigned int texSize)
	:m_textureID(0), m_width(0), m_height(0), m_bitDepth(0), m_fileLocation(nullptr)
	{
		float* texData;
		texData = stbi_loadf_from_memory(reinterpret_cast<const stbi_uc*> (texureData), texSize, &m_width, &m_height, &m_bitDepth, 0);
		if (!texData)
		{
			delete(this);
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_bitDepth == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_FLOAT, texData);
		else if (m_bitDepth == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_FLOAT, texData);
		else
		{
			//Invalid texture format
			stbi_image_free(texData);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &m_textureID);
			delete(this);
			return;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(texData);
		return;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_textureID);
	}

	void Texture::useTexture(unsigned int textureSlot)
	{
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
	unsigned int Texture::getTextureID()
	{
		return m_textureID;
	}
}