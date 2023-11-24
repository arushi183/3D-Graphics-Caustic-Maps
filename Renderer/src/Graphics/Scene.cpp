#include "Renderer/Graphics/Scene.h"
#include "Renderer/Window/Window.h"

namespace graphics
{

	Scene::Scene()
	{
		mainCamera = nullptr;
	}

	Scene::~Scene()
	{
		if (mainCamera)
			delete(mainCamera);

		for (int i = 0; i < lightList.size(); i++)
			if (lightList[i])
				delete(lightList[i]);

		for (auto& set : meshList)
			for (int i = 0; i < set.second.size(); i++)
				if (set.second[i])
					delete(set.second[i]);
	}

	void Scene::setCamera(Camera* camera)
	{
		mainCamera = camera;
	}

	void Scene::addLight(Light* light)
	{
		lightList.push_back(light);
	}

	void Scene::addGraphics(Renderable* object)
	{
		meshList[object->getShaderRef()].push_back(object);
	}

	void Scene::handleInput(const window::Input* input)
	{
		mainCamera->handleInputs(input);
	}
}