#pragma once

#include <map>
#include <vector>

#include "Renderable.h"
#include "Camera.h"
#include "Light.h"

namespace graphics
{
	struct Scene
	{
	public:
		Scene();
		~Scene();

		void setCamera(Camera* camera);
		void addLight(Light* light);
		void addGraphics(Renderable* object);

		void handleInput(const window::Input* input);
	public:
		std::map<Shader*, std::vector<Renderable*>> meshList;
		std::vector<Light*> lightList;

		Camera* mainCamera;
	};

}