#pragma once

#include <map>
#include <vector>

#include "Renderable.h"
#include "Camera.h"

namespace graphics
{
	class Light;

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