#pragma once

#include "Material.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace renderer { class Renderer; }

namespace graphics
{
#define GRAPHICS_MODEL 0
#define GRAPHICS_LINE 1

	struct Tranformation
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		Tranformation()
		{
			position = { 0,0,0 };
			rotation = { 0,0,0 };
			scale = { 1,1,1 };
		}
	};

	class Renderable
	{
	public:
		virtual void render(renderer::Renderer& render, Shader* overrideShader = nullptr) = 0;
		virtual int type() = 0;
		virtual Shader* getShaderRef() const = 0;
		virtual Material* getMaterialRef() const = 0;

		Tranformation transform;
	};

}