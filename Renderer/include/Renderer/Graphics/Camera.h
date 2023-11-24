#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderable.h"

namespace window
{
	class Window;
	class Input;
}

namespace graphics
{

	class Camera
	{
	public:
		Camera(window::Window* mainWindow);
		~Camera();

		const float* getViewMatrix() const;
		const float* getProjectionMatrix() const;

		void handleInputs(const window::Input* input);
	public:
		Tranformation transform;

	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		float m_fov;
		float m_aspectRatio;
		float m_near, m_far;

		float m_velocity;
		float m_sentivity;
	};

}