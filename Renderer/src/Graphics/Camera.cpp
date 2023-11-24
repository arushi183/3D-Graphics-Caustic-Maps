#include "Renderer/Graphics/Camera.h"

#include "Renderer/Window/Window.h"

namespace graphics
{

	Camera::Camera(window::Window* mainWindow)
		:m_velocity(15.0f), m_sentivity(0.03f)
	{
		transform.position = { -15.0f, 5.0f, 0.0f };
		transform.rotation = { 0, 0, 0 };

		m_fov = 45.0f;

		m_near = 0.1f;
		m_far = 1000.0f;
		m_aspectRatio = 1.0f;

		auto persepctiveCallBack = [this](unsigned int width, unsigned int height)
		{
			m_aspectRatio = (float)width / (float)height;
			m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
		};
		mainWindow->addSizeCallback(persepctiveCallBack);

		m_viewMatrix = glm::lookAt(transform.position, transform.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	Camera::~Camera()
	{
	}

	const float* Camera::getViewMatrix() const
	{
		return glm::value_ptr(m_viewMatrix);
	}

	const float* Camera::getProjectionMatrix() const
	{
		return glm::value_ptr(m_projectionMatrix);
	}

	void Camera::handleInputs(const window::Input* input)
	{
		double xDelta, yDelta;
		input->getMouseDelta(xDelta, yDelta);
		transform.rotation.x += (float)xDelta * m_sentivity;
		transform.rotation.y += (float)yDelta * -m_sentivity;
		transform.rotation.y = std::max(-89.0f, std::min(transform.rotation.y, 89.0f));

		glm::vec3 direction;
		direction.x = cos(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));
		direction.y = sin(glm::radians(transform.rotation.y));
		direction.z = sin(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));
		glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 forward = glm::normalize(direction);
		glm::vec3 right = glm::normalize(glm::cross(globalUp, forward));
		glm::vec3 up = glm::cross(forward, right);

		direction = glm::vec3(0.0f);

		if (input->getKeyDown(window::KEY::W))
			direction += forward;
		if (input->getKeyDown(window::KEY::S))
			direction -= forward;
		if (input->getKeyDown(window::KEY::D))
			direction -= right;
		if (input->getKeyDown(window::KEY::A))
			direction += right;
		if (input->getKeyDown(window::KEY::Q))
			direction += up;
		if (input->getKeyDown(window::KEY::E))
			direction -= up;

		if (glm::length(direction) != 0)
			direction = glm::normalize(direction) * m_velocity * input->getDeltaTime();
		transform.position += direction;

		m_viewMatrix = glm::lookAt(transform.position, transform.position + forward, up);
	}

}