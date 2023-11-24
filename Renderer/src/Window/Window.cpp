#include "Renderer/Window/Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace window
{
	void window_size_callback(GLFWwindow* window, int width, int height);
	void window_cursor_pos_callback(GLFWwindow* window, double xPos, double yPos);
	void window_key_callback(GLFWwindow* window, int key, int scancodes, int action, int mods);
	void window_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void window_scroll_callback(GLFWwindow* window, double xScroll, double yScroll);

	Window::Window(unsigned int width, unsigned int height, const char* title)
		:m_width(width), m_height(height), m_window(nullptr), m_inputs(nullptr)
	{
		if (!glfwInit())
			return;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (m_window == nullptr)
		{
			printf("Failed to create GLFW\n");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_inputs = new Input();

		//Callbacks
		glfwSetWindowSizeCallback(m_window, window_size_callback);
		glfwSetCursorPosCallback(m_window, window_cursor_pos_callback);
		glfwSetKeyCallback(m_window, window_key_callback);
		glfwSetMouseButtonCallback(m_window, window_mouse_button_callback);
		glfwSetScrollCallback(m_window, window_scroll_callback);
	}

	Window::~Window()
	{
		delete(m_inputs);

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool Window::isClosed()
	{
		m_inputs->swapInputs((float)glfwGetTime());
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		return glfwWindowShouldClose(m_window);
	}

	void Window::registerInputs()
	{
		glfwPollEvents();
	}

	void Window::addSizeCallback(FuncPtr function)
	{
		m_size_callbacks.push_back(function);
		function(m_width, m_height);
	}

	void Window::getResolution(float& width, float& height)
	{
		width = (float)m_width;
		height = (float)m_height;
	}

	void Window::updateResolution(int width, int height)
	{
		m_width = width;
		m_height = height;

		for (const FuncPtr& func : m_size_callbacks)
		{
			func(m_width, m_height);
		}
	}

	const Input* Window::getInputs() const
	{
		return m_inputs;
	}

	void Window::updateCursorPos(double x, double y)
	{
		m_inputs->setMousePos(x, y);
	}

	void Window::updateKey(int keycode, bool action)
	{
		m_inputs->updateKey(keycode, action);
	}

	void Window::updateScroll(double x, double y)
	{
		m_inputs->setScroll(x, y);
	}

	// ------------------------ Callbacks  ------------------------ 
	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Window* mainWindow = (Window*)glfwGetWindowUserPointer(window);
		mainWindow->updateResolution(width, height);
	}

	void window_cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
	{
		Window* mainWindow = (Window*)glfwGetWindowUserPointer(window);
		mainWindow->updateCursorPos(xPos, yPos);
	}

	void window_key_callback(GLFWwindow* window, int key, int scancodes, int action, int mods)
	{
		Window* mainWindow = (Window*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS)
			mainWindow->updateKey(key, true);
		else if (action == GLFW_RELEASE)
			mainWindow->updateKey(key, false);
	}

	void window_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		Window* mainWindow = (Window*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS)
			mainWindow->updateKey(button, true);
		else if (action == GLFW_RELEASE)
			mainWindow->updateKey(button, false);
	}

	void window_scroll_callback(GLFWwindow* window, double xScroll, double yScroll)
	{
		Window* mainWindow = (Window*)glfwGetWindowUserPointer(window);
		mainWindow->updateScroll(xScroll, yScroll);
	}

}