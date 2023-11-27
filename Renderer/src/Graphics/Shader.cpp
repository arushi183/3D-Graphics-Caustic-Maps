#include "Renderer/Graphics/Shader.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace graphics
{
	std::string loadFile(const char* filename)
	{
		std::ifstream file;
		file.open(filename);
		if (file)
		{
			std::stringstream strStream;
			strStream << file.rdbuf();
			std::string output = strStream.str();
			file.close();
			return output;
		}
		else
			return "";
	}

	Shader::Shader(const char* vertexShader, const char* fragmentShader)
		: m_program(0), vertexShaderCode(vertexShader), fragmentShaderCode(fragmentShader), geometryShaderCode(nullptr)
	{
		std::string vertexShaderStr = loadFile(vertexShader);
		const char* vertexShaderSource = vertexShaderStr.c_str();

		unsigned int vertexProgram = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexProgram, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexProgram);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexProgram, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexProgram, 512, NULL, infoLog);
			std::cout << "Vertex Shader Compilation Failed!" << infoLog << std::endl;
			glDeleteShader(vertexProgram);
			return;
		}

		std::string fragmentShaderStr = loadFile(fragmentShader);
		const char* fragmentShaderSource = fragmentShaderStr.c_str();

		unsigned int fragmentProgram = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentProgram, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentProgram);

		glGetShaderiv(fragmentProgram, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentProgram, 512, NULL, infoLog);
			std::cout << "Fragment Shader Compilation Failed!" << infoLog << std::endl;
			glDeleteShader(vertexProgram);
			glDeleteShader(fragmentProgram);
			return;
		}

		m_program = glCreateProgram();
		glAttachShader(m_program, vertexProgram);
		glAttachShader(m_program, fragmentProgram);
		glLinkProgram(m_program);

		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_program, 512, NULL, infoLog);
			std::cout << "Shader Link Failed!" << infoLog << std::endl;
			m_program = 0;

			glDeleteShader(vertexProgram);
			glDeleteShader(fragmentProgram);

			glDeleteProgram(m_program);
			return;
		}

		glDeleteShader(vertexProgram);
		glDeleteShader(fragmentProgram);
	}

	Shader::Shader(const char* vertexShader, const char* fragmentShader, const char* geometryShader)
		: m_program(0), vertexShaderCode(vertexShader), fragmentShaderCode(fragmentShader), geometryShaderCode(geometryShader)
	{
		std::string vertexShaderStr = loadFile(vertexShader);
		const char* vertexShaderSource = vertexShaderStr.c_str();

		unsigned int vertexProgram = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexProgram, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexProgram);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexProgram, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexProgram, 512, NULL, infoLog);
			std::cout << "Vertex Shader Compilation Failed!" << infoLog << std::endl;
			glDeleteShader(vertexProgram);
			return;
		}

		std::string fragmentShaderStr = loadFile(fragmentShader);
		const char* fragmentShaderSource = fragmentShaderStr.c_str();

		unsigned int fragmentProgram = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentProgram, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentProgram);

		glGetShaderiv(fragmentProgram, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentProgram, 512, NULL, infoLog);
			std::cout << "Fragment Shader Compilation Failed!" << infoLog << std::endl;
			glDeleteShader(vertexProgram);
			glDeleteShader(fragmentProgram);
			return;
		}

		std::string geometryShaderStr = loadFile(fragmentShader);
		const char* geometryShaderSource = geometryShaderStr.c_str();

		unsigned int geometryProgram = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryProgram, 1, &geometryShaderSource, NULL);
		glCompileShader(geometryProgram);

		glGetShaderiv(geometryProgram, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometryProgram, 512, NULL, infoLog);
			std::cout << "Geometry Shader Compilation Failed!" << infoLog << std::endl;
			glDeleteShader(vertexProgram);
			glDeleteShader(fragmentProgram);
			glDeleteShader(geometryProgram);
			return;
		}

		m_program = glCreateProgram();
		glAttachShader(m_program, vertexProgram);
		glAttachShader(m_program, geometryProgram);
		glAttachShader(m_program, fragmentProgram);
		glLinkProgram(m_program);

		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_program, 512, NULL, infoLog);
			std::cout << "Shader Link Failed!" << infoLog << std::endl;
			m_program = 0;

			glDeleteShader(vertexProgram);
			glDeleteShader(fragmentProgram);

			glDeleteProgram(m_program);
			return;
		}

		glDeleteShader(vertexProgram);
		glDeleteShader(fragmentProgram);
		glDeleteShader(geometryProgram);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_program);
	}

	void Shader::bind()
	{
		glUseProgram(m_program);
	}

	void Shader::unbind()
	{
		glUseProgram(0);
	}

	bool Shader::setUniformInt(const char* uniform, int value)
	{
		unsigned int location = 0;
		if (m_uniformLookup.find(uniform) != m_uniformLookup.end())
			location = m_uniformLookup[uniform];
		else
			location = m_uniformLookup[uniform] = glGetUniformLocation(m_program, uniform);

		glUniform1i(location, value);
		return true;
	}

	bool Shader::setUniformFloat(const char* uniform, float value)
	{
		unsigned int location = 0;
		if (m_uniformLookup.find(uniform) != m_uniformLookup.end())
			location = m_uniformLookup[uniform];
		else
			location = m_uniformLookup[uniform] = glGetUniformLocation(m_program, uniform);

		glUniform1f(location, value);
		return true;
	}

	bool Shader::setUniformBool(const char* uniform, bool value)
	{
		unsigned int location = 0;
		if (m_uniformLookup.find(uniform) != m_uniformLookup.end())
			location = m_uniformLookup[uniform];
		else
			location = m_uniformLookup[uniform] = glGetUniformLocation(m_program, uniform);

		glUniform1i(location, (int)value);
		return true;
	}

	bool Shader::setUniformVec2(const char* uniform, const float* value)
	{
		unsigned int location = 0;
		if (m_uniformLookup.find(uniform) != m_uniformLookup.end())
			location = m_uniformLookup[uniform];
		else
			location = m_uniformLookup[uniform] = glGetUniformLocation(m_program, uniform);

		glUniform2f(location, value[0], value[1]);
		return true;
	}

	bool Shader::setUniformVec3(const char* uniform, const float* value)
	{
		unsigned int location = 0;
		if (m_uniformLookup.find(uniform) != m_uniformLookup.end())
			location = m_uniformLookup[uniform];
		else
			location = m_uniformLookup[uniform] = glGetUniformLocation(m_program, uniform);

		glUniform3f(location, value[0], value[1], value[2]);
		return true;
	}

	bool Shader::setUniformVec4(const char* uniform, const float* value)
	{
		unsigned int location = 0;
		if (m_uniformLookup.find(uniform) != m_uniformLookup.end())
			location = m_uniformLookup[uniform];
		else
			location = m_uniformLookup[uniform] = glGetUniformLocation(m_program, uniform);

		glUniform4f(location, value[0], value[1], value[2], value[3]);
		return true;
	}

	bool Shader::setUnifromMat4f(const char* uniform, const float* value)
	{
		unsigned int location = 0;
		if (m_uniformLookup.find(uniform) != m_uniformLookup.end())
			location = m_uniformLookup[uniform];
		else
			location = m_uniformLookup[uniform] = glGetUniformLocation(m_program, uniform);

		glUniformMatrix4fv(location, 1, GL_FALSE, value);
		return true;
	}
}